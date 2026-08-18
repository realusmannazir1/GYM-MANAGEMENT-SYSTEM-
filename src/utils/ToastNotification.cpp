#include "utils/ToastNotification.h"
#include <QLabel>
#include <QTimer>
#include <QApplication>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>

namespace FitCore {

void ToastNotification::show(QWidget *parent, const QString &text, ToastType type) {
    QWidget *anchor = parent ? parent->window() : QApplication::activeWindow();
    if (!anchor) return;

    QLabel *toast = new QLabel(anchor);
    toast->setText(text);
    toast->setWordWrap(true);
    toast->setStyleSheet(styleFor(type));
    toast->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    toast->setAttribute(Qt::WA_ShowWithoutActivating);
    toast->setAttribute(Qt::WA_TransparentForMouseEvents);
    toast->adjustSize();

    const int maxWidth = qBound(280, anchor->width() - 100, 460);
    if (toast->width() > maxWidth) {
        toast->setMaximumWidth(maxWidth);
        toast->adjustSize();
    }

    const int margin = 24;
    toast->move(anchor->frameGeometry().right() - toast->width() - margin,
                anchor->frameGeometry().top() + margin);

    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(toast);
    toast->setGraphicsEffect(effect);
    QPropertyAnimation *fadeIn = new QPropertyAnimation(effect, "opacity", toast);
    fadeIn->setDuration(220);
    fadeIn->setStartValue(0.0);
    fadeIn->setEndValue(1.0);
    fadeIn->start(QAbstractAnimation::DeleteWhenStopped);

    toast->show();

    QTimer::singleShot(3400, toast, [toast, effect]() {
        QPropertyAnimation *fadeOut = new QPropertyAnimation(effect, "opacity", toast);
        fadeOut->setDuration(380);
        fadeOut->setStartValue(effect->opacity());
        fadeOut->setEndValue(0.0);
        QObject::connect(fadeOut, &QPropertyAnimation::finished, toast, &QWidget::close);
        QObject::connect(fadeOut, &QPropertyAnimation::finished, toast, &QObject::deleteLater);
        fadeOut->start(QAbstractAnimation::DeleteWhenStopped);
    });
}

QString ToastNotification::styleFor(ToastType type) {
    const QString base = "border-radius: 8px; padding: 12px 18px; font-size: 13px; font-weight: 600; color: #FFFFFF;";
    switch (type) {
    case ToastType::Success:
        return QString("QLabel { background-color: #10B981; %1 }").arg(base);
    case ToastType::Error:
        return QString("QLabel { background-color: #EF4444; %1 }").arg(base);
    case ToastType::Info:
        return QString("QLabel { background-color: #3B82F6; %1 }").arg(base);
    case ToastType::Warning:
        return QString("QLabel { background-color: #F59E0B; %1 }").arg(base);
    }
    return QString("QLabel { background-color: #3B82F6; %1 }").arg(base);
}

} // namespace FitCore
