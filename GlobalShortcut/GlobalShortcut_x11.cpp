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
#include <QProcessEnvironment>

namespace qtlib
{

//-------------------------------------------------------------------------------------------------
namespace
{

/**
 * maskModifiers
 *
 * Mask        | Value | Key
 * ------------+-------+------------
 * ShiftMask   |     1 | Shift
 * LockMask    |     2 | Caps Lock
 * ControlMask |     4 | Ctrl
 * Mod1Mask    |     8 | Alt
 * Mod2Mask    |    16 | Num Lock
 * Mod3Mask    |    32 | Scroll Lock
 * Mod4Mask    |    64 | Windows
 * Mod5Mask    |   128 | ???
 */
const QVector<quint32> maskModifiers =
    QVector<quint32>() << 0 << Mod2Mask << LockMask << (Mod2Mask | LockMask);
//-------------------------------------------------------------------------------------------------
class QxtX11ErrorHandler
{
public:
    static Bool isError;

    QxtX11ErrorHandler()
    {
        isError = False;

        _errorHandlerLast = ::XSetErrorHandler(qxtX11ErrorHandler);
        qTEST_PTR(_errorHandlerLast);
    }

    ~QxtX11ErrorHandler()
    {
        x11_error_handler_t errorHandlerLast = ::XSetErrorHandler(_errorHandlerLast);
        qTEST_PTR(errorHandlerLast);
    }

private:
    using x11_error_handler_t = std::add_pointer<Bool(Display *display, XErrorEvent *event)>::type;

    x11_error_handler_t _errorHandlerLast {};

    static int
    qxtX11ErrorHandler(
        Display     *a_display,
        XErrorEvent *a_event
    )
    {
        qDebug() << "::::: " << __FUNCTION__ << " :::::ы";

        qTEST_PTR(a_display);
        qTEST_PTR(a_event);

        if (a_event->error_code == Success) {
            isError = False;
            qDebug() << "Success";
            return 0;
        }

        isError = True;
        errorText(a_display, a_event);

        switch (a_event->error_code) {
        case BadAccess:
        case BadValue:
        case BadWindow:
        // case BadMatch:
            if (a_event->request_code == 33 /* X_GrabKey */ ||
                a_event->request_code == 34 /* X_UngrabKey */)
            {
                ///
            }
        }

        return 1;
    }

    static
    void errorText(
        Display     *a_display,
        XErrorEvent *a_event
    )
    {
        char errorText[255 + 1] {};
        ::XGetErrorText(a_display, a_event->error_code, errorText, sizeof(errorText) - 1);

        // could be the value of $DISPLAY or nullptr
        const char *displayName = QProcessEnvironment::systemEnvironment().value("DISPLAY", ":0.0")
                                        .toStdString().c_str();

        qDebug() << "QxtX11ErrorHandler: "
            << "Dispaly: " << ::XDisplayName(displayName) << "\n"
            << "Error:   " << a_event->error_code << " - " << errorText << "\n"
            << "Event: "   << a_event->type;
    }
};
//-------------------------------------------------------------------------------------------------
Bool QxtX11ErrorHandler::isError {False};

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
        // could be the value of $DISPLAY or nullptr
        const char *displayName = QProcessEnvironment::systemEnvironment().value("DISPLAY", ":0.0")
                                        .toStdString().c_str();

        /// _display = ::XOpenDisplay(displayName);
        _display = ::XOpenDisplay(NULL);
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

    /**
     * http://incise.org/xlib-key-passing.html
     */
    bool
    grabKey(
        quint32 a_keycode,
        quint32 a_modifiers,
        Window  a_window
    )
    {
        QxtX11ErrorHandler errorHandler;
        qDebug() << "grabKey 1: " << qTRACE_VAR(errorHandler.isError);

        for (int i = 0; !errorHandler.isError && i < maskModifiers.size(); ++ i) {
            int iRv = ::XGrabKey(display(), a_keycode, a_modifiers | maskModifiers[i], a_window, True,
                GrabModeAsync, GrabModeAsync);
            // qTEST(iRv == 0);
            // if (iRv != 0) {
                qDebug() << "XGrabKey: " << qTRACE_VAR(iRv);
            // }
        }

        qDebug() << "grabKey 2: " << qTRACE_VAR(errorHandler.isError);
        if (errorHandler.isError) {
            bool bRv = ungrabKey(a_keycode, a_modifiers, a_window);
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
        QxtX11ErrorHandler errorHandler;

        for (const auto &maskMods : maskModifiers) {
            int iRv = ::XUngrabKey(display(), a_keycode, a_modifiers | maskMods, a_window);
            if (iRv != 0) {
                qDebug() << "XUngrabKey: " << qTRACE_VAR(iRv);
            }
            /// qTEST(iRv == 0);
        }

        return !errorHandler.isError;
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
    qTEST(a_eventType.size() > 0);
    qTEST_PTR(a_message);
    qTEST_PTR(a_result);

    Q_UNUSED(a_result);

    xcb_key_press_event_t *kev {};

    // qDebug() << qTRACE_VAR(a_eventType);

    if (a_eventType == "xcb_generic_event_t") {
        xcb_generic_event_t *event = static_cast<xcb_generic_event_t *>(a_message);
        // qDebug() << qTRACE_VAR(event->response_type);
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
    quint32 native {};

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
        return {};
    }

    KeySym keysym = ::XStringToKeysym(QKeySequence(a_key).toString().toLatin1().data());
    if (keysym == NoSymbol) {
        qTEST(false);
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
    /// qTEST(a_nativeKey > 0);
    /// qTEST(a_nativeMods > 0);

    QxtX11Data x11;

    if ( !x11.isValid() ) {
        qDebug() << qTRACE_VAR(__FUNCTION__) << ": " << qTRACE_VAR(__LINE__) << " - FAIL";
        return false;
    }

    if ( !x11.grabKey(a_nativeKey, a_nativeMods, x11.rootWindow()) ) {
        qDebug() << qTRACE_VAR(__FUNCTION__) << ": " << qTRACE_VAR(__LINE__) << " - FAIL";
        return false;
    }

    qDebug() << qTRACE_VAR(__FUNCTION__) << ": " << qTRACE_VAR(__LINE__) << " - OK";

    return true;
}
//-------------------------------------------------------------------------------------------------
bool
GlobalShortcut_impl::unregisterShortcut(
    quint32 a_nativeKey,
    quint32 a_nativeMods
)
{
    QxtX11Data x11;

    if ( !x11.isValid() ) {
        qDebug() << qTRACE_VAR(__FUNCTION__) << ": " << qTRACE_VAR(__LINE__) << " - FAIL";
        return false;
    }

    if ( !x11.ungrabKey(a_nativeKey, a_nativeMods, x11.rootWindow()) ) {
        qDebug() << qTRACE_VAR(__FUNCTION__) << ": " << qTRACE_VAR(__LINE__) << " - FAIL";
        return false;
    }

    qDebug() << qTRACE_VAR(__FUNCTION__) << ": " << qTRACE_VAR(__LINE__) << " - OK";

    return true;
}
//-------------------------------------------------------------------------------------------------

} // namespace qtlib
