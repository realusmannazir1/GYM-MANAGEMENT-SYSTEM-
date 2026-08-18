#ifndef TOASTNOTIFICATION_H
#define TOASTNOTIFICATION_H

#include <QWidget>
#include <QString>

namespace FitCore {

enum class ToastType { Success, Error, Info, Warning };

// Light-weight, non-blocking notification that appears at the top-right corner
// of the given parent window (like a web-app toast notification).
class ToastNotification {
public:
    static void show(QWidget *parent, const QString &text, ToastType type = ToastType::Success);

private:
    static QString styleFor(ToastType type);
};

} // namespace FitCore

#endif // TOASTNOTIFICATION_H
