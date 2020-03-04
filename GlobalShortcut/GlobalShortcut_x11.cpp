/**
 * \file
 * \brief
 */


#include "GlobalShortcut_impl.h"

// Qt 5.11.1: don't supprt Qt Platform Abstraction (QPA)
#define QT_QPA 0

#if QT_QPA
#include <qpa/qplatformnativeinterface.h>
#endif

#include <X11/Xlib.h>
#include <xcb/xcb.h>


namespace qtlib
{

//-------------------------------------------------------------------------------------------------
namespace
{

const QVector<quint32> maskModifiers =
    QVector<quint32>() << 0 << Mod2Mask << LockMask << (Mod2Mask | LockMask);

using X11ErrorHandler = std::add_pointer<int(Display *display, XErrorEvent *event)>::type;
//-------------------------------------------------------------------------------------------------
class QxtX11ErrorHandler
{
public:
    QxtX11ErrorHandler()
    {
        error                 = false;
        _previousErrorHandler = ::XSetErrorHandler(qxtX11ErrorHandler);
    }

    ~QxtX11ErrorHandler()
    {
        ::XSetErrorHandler(_previousErrorHandler);
    }

    static bool error;

    static int
    qxtX11ErrorHandler(
        Display     *a_display,
        XErrorEvent *a_event
    )
    {
        switch (a_event->error_code) {
        case BadAccess:
        case BadValue:
        case BadWindow:
        // case BadMatch:
            if (a_event->request_code == 33 /* X_GrabKey */ ||
                a_event->request_code == 34 /* X_UngrabKey */)
            {
                error = true;

                // TODO:
                char errorText[255 + 1] = {0};
                ::XGetErrorText(a_display, a_event->error_code, errorText, sizeof(errorText) - 1);

                qDebug() << "QxtX11ErrorHandler: " << errorText;
            }
        }

        return 0;
    }

private:
    X11ErrorHandler _previousErrorHandler {};
};
//-------------------------------------------------------------------------------------------------
bool QxtX11ErrorHandler::error = false;

class QxtX11Data
{
public:
    QxtX11Data()
    {
    #if QT_QPA
        QPlatformNativeInterface *native  = qApp->platformNativeInterface();
        void                     *display = native->nativeResourceForScreen(QByteArray("display"),
             QGuiApplication::primaryScreen());
        _display = reinterpret_cast<Display *>(display);
    #else
        char *displayName {};  // could be the value of $DISPLAY

        _display = ::XOpenDisplay(displayName);
        if (_display == nullptr) {
            qDebug() << "XOpenDisplay: " << ::XDisplayName(displayName);
        }
    #endif
    }

    bool
    isValid()
    {
        return (_display != Q_NULLPTR);
    }

    Display *
    display()
    {
        qTEST(isValid());

        return _display;
    }

    Window
    rootWindow()
    {
        return DefaultRootWindow(display());
    }

    bool
    grabKey(
        quint32 a_keycode,
        quint32 a_modifiers,
        Window  a_window
    )
    {
        int  iRv {};
        bool bRv {};
        QxtX11ErrorHandler errorHandler;

        for (int i = 0; !errorHandler.error && i < maskModifiers.size(); ++ i) {
            iRv = ::XGrabKey(display(), a_keycode, a_modifiers | maskModifiers[i], a_window, True,
                GrabModeAsync, GrabModeAsync);
             qTEST(iRv == 0);
        }

        if (errorHandler.error) {
            bRv = ungrabKey(a_keycode, a_modifiers, a_window);
            qTEST(bRv);

            return false;
        }

        return true;
    }

    bool
    ungrabKey(
        quint32 a_keycode,
        quint32 a_modifiers,
        Window  a_window
    )
    {
        int iRv {};
        QxtX11ErrorHandler errorHandler;

        Q_FOREACH(quint32 maskMods, maskModifiers) {
            iRv = ::XUngrabKey(display(), a_keycode, a_modifiers | maskMods, a_window);
            qTEST(iRv == 0);
        }

        return !errorHandler.error;
    }

private:
    Display *_display {};
};
//-------------------------------------------------------------------------------------------------

} // namespace

//-------------------------------------------------------------------------------------------------
bool
GlobalShortcut_impl::nativeEventFilter(
    const QByteArray &a_eventType,
    void             *a_message,
    long             *a_result
)
{
    Q_UNUSED(a_result);

    xcb_key_press_event_t *kev = Q_NULLPTR;

    qDebug() << qTRACE_VAR(a_eventType);

    if (a_eventType == "xcb_generic_event_t") {
        xcb_generic_event_t *event = static_cast<xcb_generic_event_t *>(a_message);
        qDebug() << qTRACE_VAR(event->response_type);
        if ((event->response_type & 127) == XCB_KEY_PRESS) {
            kev = static_cast<xcb_key_press_event_t *>(a_message);
        }
    }

    if (kev != Q_NULLPTR) {
        unsigned int keycode  = kev->detail;
        unsigned int keystate = 0;

        if (kev->state & XCB_MOD_MASK_1) {
            keystate |= Mod1Mask;
        }
        if (kev->state & XCB_MOD_MASK_CONTROL) {
            keystate |= ControlMask;
        }
        if (kev->state & XCB_MOD_MASK_4) {
            keystate |= Mod4Mask;
        }
        if (kev->state & XCB_MOD_MASK_SHIFT) {
            keystate |= ShiftMask;
        }

        activateShortcut(keycode,
            // Mod1Mask == Alt, Mod4Mask == Meta
            keystate & (ShiftMask | ControlMask | Mod1Mask | Mod4Mask));
    }

    return false;
}
//-------------------------------------------------------------------------------------------------
quint32
GlobalShortcut_impl::nativeModifiers(
    Qt::KeyboardModifiers a_modifiers
)
{
    // ShiftMask, LockMask, ControlMask, Mod1Mask, Mod2Mask, Mod3Mask, Mod4Mask, and Mod5Mask
    quint32 native = 0;

    if (a_modifiers & Qt::ShiftModifier) {
        native |= ShiftMask;
    }
    if (a_modifiers & Qt::ControlModifier) {
        native |= ControlMask;
    }
    if (a_modifiers & Qt::AltModifier) {
        native |= Mod1Mask;
    }
    if (a_modifiers & Qt::MetaModifier) {
        native |= Mod4Mask;
    }

    // TODO: resolve these?
    // if (modifiers & Qt::MetaModifier)
    // if (modifiers & Qt::KeypadModifier)
    // if (modifiers & Qt::GroupSwitchModifier)

    return native;
}
//-------------------------------------------------------------------------------------------------
quint32
GlobalShortcut_impl::nativeKeycode(
    Qt::Key a_key
)
{
    QxtX11Data x11;
    if ( !x11.isValid() ) {
        return 0;
    }

    KeySym keysym = ::XStringToKeysym(QKeySequence(a_key).toString().toLatin1().data());
    if (keysym == NoSymbol) {
        keysym = static_cast<ushort>(a_key);
    }

    return ::XKeysymToKeycode(x11.display(), keysym);
}
//-------------------------------------------------------------------------------------------------
bool
GlobalShortcut_impl::registerShortcut(
    quint32 a_nativeKey,
    quint32 a_nativeMods
)
{
    QxtX11Data x11;

    return x11.isValid() && x11.grabKey(a_nativeKey, a_nativeMods, x11.rootWindow());
}
//-------------------------------------------------------------------------------------------------
bool
GlobalShortcut_impl::unregisterShortcut(
    quint32 a_nativeKey,
    quint32 a_nativeMods
)
{
    QxtX11Data x11;

    return x11.isValid() && x11.ungrabKey(a_nativeKey, a_nativeMods, x11.rootWindow());
}
//-------------------------------------------------------------------------------------------------

} // namespace qtlib
