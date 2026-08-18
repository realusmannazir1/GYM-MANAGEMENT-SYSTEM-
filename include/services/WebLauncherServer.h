#ifndef WEBLAUNCHERSERVER_H
#define WEBLAUNCHERSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QByteArray>
#include <QMap>
#include "models/User.h"

namespace FitCore {

// Tiny single-threaded HTTP server used as the "HTML launcher".
// It serves a modern login page (pure HTML/CSS/JS) in the user's browser and
// validates credentials against the C++ backend. On success it emits
// loginSucceeded(), and main.cpp opens the native desktop dashboard.
class WebLauncherServer : public QObject {
    Q_OBJECT
public:
    explicit WebLauncherServer(QObject *parent = nullptr);

    bool start(int preferredPort = 8770);
    int port() const { return m_port; }
    QString launcherUrl() const;

signals:
    void loginSucceeded(const User &user);

private slots:
    void onNewConnection();
    void onReadyRead();
    void onDisconnected();

private:
    void handleRequest(QTcpSocket *socket, const QByteArray &request);
    QByteArray buildResponse(const QByteArray &status, const QByteArray &contentType, const QByteArray &body) const;
    static QByteArray percentDecode(const QByteArray &input);
    static QByteArray jsonEscape(const QString &text);

    QTcpServer m_server;
    QMap<QTcpSocket *, QByteArray> m_buffers;
    int m_port{0};
};

} // namespace FitCore

#endif // WEBLAUNCHERSERVER_H