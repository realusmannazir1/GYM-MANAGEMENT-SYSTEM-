#include "services/WebLauncherServer.h"
#include "services/AuthenticationService.h"
#include <QHostAddress>
#include <QUrl>
#include <QStringList>

namespace FitCore {

namespace {

QByteArray launcherPageHtml() {
    return R"HTML(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>FitCore — Secure Sign In</title>
<style>
:root { --primary:#2563EB; --primary-dark:#1D4ED8; --primary-light:#3B82F6; --ink:#0F172A; --muted:#64748B; --line:#E2E8F0; }
* { margin:0; padding:0; box-sizing:border-box; }
body {
  font-family:'Segoe UI','SF Pro Text','Roboto',sans-serif;
  background:linear-gradient(135deg,#0F172A 0%,#1E3A5F 50%,#1D4ED8 100%);
  min-height:100vh; display:flex; align-items:center; justify-content:center; padding:24px;
}
.card {
  background:#FFFFFF; border-radius:20px; box-shadow:0 30px 70px rgba(2,6,23,.45);
  width:100%; max-width:960px; display:flex; overflow:hidden; min-height:540px;
}
.hero {
  flex:1.1; background:linear-gradient(160deg,#1D4ED8 0%,#2563EB 60%,#3B82F6 100%);
  color:#FFFFFF; padding:44px 40px; display:flex; flex-direction:column; justify-content:space-between;
}
.hero .badge { display:inline-block; background:rgba(255,255,255,.16); border:1px solid rgba(255,255,255,.3); padding:6px 14px; border-radius:999px; font-size:11px; font-weight:700; letter-spacing:2px; }
.hero h1 { font-size:30px; font-weight:800; margin-top:18px; letter-spacing:.5px; }
.hero .tagline { font-size:13.5px; line-height:1.6; color:#DBEAFE; margin-top:10px; }
.hero .features { margin-top:22px; display:flex; flex-direction:column; gap:10px; font-size:12.5px; color:#EFF6FF; }
.hero .features div { display:flex; align-items:center; gap:8px; }
.hero .features span { width:18px; height:18px; border-radius:50%; background:rgba(16,185,129,.9); color:#FFFFFF; font-size:11px; display:inline-flex; align-items:center; justify-content:center; font-weight:800; }
.hero .foot { font-size:11px; color:#BFDBFE; margin-top:26px; }
.form-side { flex:1; padding:44px 40px; display:flex; flex-direction:column; justify-content:center; }
.form-side h2 { font-size:22px; font-weight:800; color:var(--ink); }
.form-side .sub { font-size:13px; color:var(--muted); margin-top:4px; margin-bottom:22px; }
.error { display:none; background:#FEE2E2; color:#991B1B; padding:10px 12px; border-radius:8px; font-size:13px; margin-bottom:16px; }
label { font-size:12.5px; font-weight:600; color:var(--ink); display:block; margin-top:14px; }
input {
  width:100%; padding:12px 14px; border:1.5px solid var(--line); border-radius:10px;
  font-size:14px; color:var(--ink); margin-top:6px; outline:none; background:#F8FAFC; transition:all .15s ease;
}
input:focus { border-color:var(--primary); background:#FFFFFF; box-shadow:0 0 0 3px rgba(37,99,235,.15); }
.presets { display:flex; flex-wrap:wrap; gap:6px; margin-top:16px; align-items:center; font-size:12px; color:var(--muted); }
.presets button {
  background:#EFF6FF; color:var(--primary-dark); border:1px solid #BFDBFE; border-radius:999px;
  padding:5px 12px; font-size:11.5px; font-weight:600; cursor:pointer; transition:all .15s ease;
}
.presets button:hover { background:var(--primary); color:#FFFFFF; border-color:var(--primary); }
.btn {
  width:100%; margin-top:22px; padding:14px; border:none; border-radius:10px; cursor:pointer;
  background:linear-gradient(90deg,var(--primary),var(--primary-dark));
  color:#FFFFFF; font-size:14.5px; font-weight:700; letter-spacing:.3px; transition:filter .15s ease;
}
.btn:hover { filter:brightness(1.08); }
.btn:disabled { opacity:.75; cursor:wait; }
.hint { font-size:11.5px; color:#94A3B8; margin-top:18px; line-height:1.5; }
@media (max-width:780px) { .hero { display:none; } .card { max-width:430px; min-height:auto; } }
</style>
</head>
<body>
<div class="card">
  <div class="hero">
    <div><span class="badge">FITCORE ENTERPRISE</span></div>
    <div>
      <h1>Welcome back</h1>
      <p class="tagline">Streamline member management, billing, attendance, trainers and analytics — all from one powerful desktop console.</p>
      <div class="features">
        <div><span>✓</span> Member &amp; membership tracking</div>
        <div><span>✓</span> Payments, receipts &amp; financial reports</div>
        <div><span>✓</span> Fast check-in attendance terminal</div>
        <div><span>✓</span> Automatic expiry alerts &amp; backups</div>
      </div>
    </div>
    <div class="foot">⚡ FitCore v1.0.0 Enterprise &bull; C++17 + Qt 6 + SQLite engine</div>
  </div>
  <div class="form-side">
    <h2>Sign in to FitCore</h2>
    <p class="sub">Launch the desktop management dashboard.</p>
    <div class="error" id="error"></div>
    <form id="loginForm">
      <label for="username">Username</label>
      <input id="username" autocomplete="username" placeholder="e.g. admin" required>
      <label for="password">Password</label>
      <input id="password" type="password" autocomplete="current-password" placeholder="&#8226;&#8226;&#8226;&#8226;&#8226;&#8226;&#8226;&#8226;" required>
      <div class="presets">Quick fill:
        <button type="button" data-u="admin" data-p="admin123">Admin</button>
        <button type="button" data-u="manager" data-p="manager123">Manager</button>
        <button type="button" data-u="receptionist" data-p="receptionist123">Receptionist</button>
        <button type="button" data-u="trainer1" data-p="trainer123">Trainer</button>
      </div>
      <button class="btn" type="submit" id="btn"><span id="btnText">Launch FitCore Dashboard</span></button>
    </form>
    <p class="hint">The FitCore desktop dashboard opens automatically after a successful sign-in. You can close this tab at any time.</p>
  </div>
</div>
<script>
var form=document.getElementById('loginForm'),btn=document.getElementById('btn'),btnText=document.getElementById('btnText'),errBox=document.getElementById('error');
function showError(msg){ errBox.textContent=msg; errBox.style.display='block'; }
function doLogin(e){
  e.preventDefault(); errBox.style.display='none';
  var username=document.getElementById('username').value.trim();
  var password=document.getElementById('password').value;
  if(!username||!password){ showError('Please enter both username and password.'); return false; }
  btn.disabled=true; btnText.textContent='Verifying credentials…';
  var body='username='+encodeURIComponent(username)+'&password='+encodeURIComponent(password);
  fetch('/api/login',{method:'POST',headers:{'Content-Type':'application/x-www-form-urlencoded'},body:body})
    .then(function(r){ return r.json(); })
    .then(function(data){
      if(data.success){ btnText.textContent='✓ Login successful — opening FitCore…'; }
      else { btn.disabled=false; btnText.textContent='Launch FitCore Dashboard'; showError(data.message||'Login failed. Please try again.'); }
    })
    .catch(function(){
      btn.disabled=false; btnText.textContent='Launch FitCore Dashboard';
      showError('Could not reach the FitCore service. Is the desktop app still running?');
    });
  return false;
}
form.addEventListener('submit',doLogin);
document.querySelectorAll('.presets button').forEach(function(b){
  b.addEventListener('click',function(){ document.getElementById('username').value=b.getAttribute('data-u'); document.getElementById('password').value=b.getAttribute('data-p'); });
});
</script>
</body>
</html>
)HTML";
}

} // namespace

WebLauncherServer::WebLauncherServer(QObject *parent)
    : QObject(parent) {
    connect(&m_server, &QTcpServer::newConnection, this, &WebLauncherServer::onNewConnection);
}

bool WebLauncherServer::start(int preferredPort) {
    if (m_server.listen(QHostAddress::LocalHost, preferredPort)) {
        m_port = m_server.serverPort();
        return true;
    }
    if (m_server.listen(QHostAddress::LocalHost, 0)) {
        m_port = m_server.serverPort();
        return true;
    }
    return false;
}

QString WebLauncherServer::launcherUrl() const {
    return QString("http://127.0.0.1:%1/").arg(m_port);
}

void WebLauncherServer::onNewConnection() {
    while (m_server.hasPendingConnections()) {
        QTcpSocket *socket = m_server.nextPendingConnection();
        connect(socket, &QTcpSocket::readyRead, this, &WebLauncherServer::onReadyRead);
        connect(socket, &QTcpSocket::disconnected, this, &WebLauncherServer::onDisconnected);
        m_buffers.insert(socket, QByteArray());
    }
}

void WebLauncherServer::onReadyRead() {
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (!socket) return;
    auto it = m_buffers.find(socket);
    if (it == m_buffers.end()) return;

    it->append(socket->readAll());

    const int headerEnd = it->indexOf("\r\n\r\n");
    if (headerEnd < 0) return;

    const QByteArray headerBlock = it->left(headerEnd);
    int contentLength = 0;
    for (const QByteArray &line : headerBlock.split('\n')) {
        if (line.toLower().startsWith("content-length:")) {
            contentLength = line.mid(15).trimmed().toInt();
            break;
        }
    }

    const int total = headerEnd + 4 + contentLength;
    if (it->size() < total) return;

    handleRequest(socket, it->left(total));
    socket->flush();
    socket->disconnectFromHost();
}

void WebLauncherServer::onDisconnected() {
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (socket) {
        m_buffers.remove(socket);
        socket->deleteLater();
    }
}

void WebLauncherServer::handleRequest(QTcpSocket *socket, const QByteArray &request) {
    const QList<QByteArray> lines = request.split('\n');
    if (lines.isEmpty()) {
        socket->write(buildResponse("400 Bad Request", "text/plain; charset=utf-8", "Bad request"));
        return;
    }

    const QList<QByteArray> parts = lines.first().trimmed().split(' ');
    if (parts.size() < 2) {
        socket->write(buildResponse("400 Bad Request", "text/plain; charset=utf-8", "Bad request"));
        return;
    }

    const QByteArray method = parts.at(0).toUpper();
    const QByteArray path = parts.at(1);

    if (method == "GET" && (path == "/" || path == "/launcher" || path.startsWith("/index"))) {
        socket->write(buildResponse("200 OK", "text/html; charset=utf-8", launcherPageHtml()));
        return;
    }

    if (method == "GET" && path == "/api/health") {
        socket->write(buildResponse("200 OK", "application/json; charset=utf-8",
                                    "{\"status\":\"ok\",\"service\":\"FitCore HTML Launcher\"}"));
        return;
    }

    if (method == "POST" && path == "/api/login") {
        const int headerEnd = request.indexOf("\r\n\r\n");
        const QByteArray body = headerEnd >= 0 ? request.mid(headerEnd + 4) : QByteArray();

        QMap<QByteArray, QByteArray> fields;
        for (const QByteArray &pair : body.split('&')) {
            const int eq = pair.indexOf('=');
            if (eq > 0) {
                fields.insert(percentDecode(pair.left(eq)), percentDecode(pair.mid(eq + 1)));
            }
        }

        const QString username = QString::fromUtf8(fields.value("username"));
        const QString password = QString::fromUtf8(fields.value("password"));

        AuthResult result = AuthenticationService::instance().login(username, password);
        if (result.success) {
            const QByteArray json = "{\"success\":true,\"message\":\"Login successful!\",\"user\":\""
                + jsonEscape(result.user.value().getFullName()) + "\"}";
            socket->write(buildResponse("200 OK", "application/json; charset=utf-8", json));
            emit loginSucceeded(result.user.value());
        } else {
            const QByteArray json = "{\"success\":false,\"message\":\"" + jsonEscape(result.message) + "\"}";
            socket->write(buildResponse("200 OK", "application/json; charset=utf-8", json));
        }
        return;
    }

    socket->write(buildResponse("404 Not Found", "text/plain; charset=utf-8", "Not found"));
}

QByteArray WebLauncherServer::buildResponse(const QByteArray &status, const QByteArray &contentType, const QByteArray &body) const {
    QByteArray response;
    response += "HTTP/1.1 " + status + "\r\n";
    response += "Content-Type: " + contentType + "\r\n";
    response += "Content-Length: " + QByteArray::number(body.size()) + "\r\n";
    response += "Cache-Control: no-store\r\n";
    response += "Connection: close\r\n\r\n";
    response += body;
    return response;
}

QByteArray WebLauncherServer::percentDecode(const QByteArray &input) {
    QByteArray output;
    output.reserve(input.size());
    for (int i = 0; i < input.size(); ++i) {
        const char c = input.at(i);
        if (c == '+') {
            output.append(' ');
        } else if (c == '%' && i + 2 < input.size()) {
            bool ok = false;
            const int value = input.mid(i + 1, 2).toInt(&ok, 16);
            if (ok) {
                output.append(static_cast<char>(value));
                i += 2;
            } else {
                output.append(c);
            }
        } else {
            output.append(c);
        }
    }
    return output;
}

QByteArray WebLauncherServer::jsonEscape(const QString &text) {
    QByteArray output;
    const QByteArray utf8 = text.toUtf8();
    for (const char c : utf8) {
        switch (c) {
        case '"': output += "\\\""; break;
        case '\\': output += "\\\\"; break;
        case '\n': output += "\\n"; break;
        case '\r': output += "\\r"; break;
        case '\t': output += "\\t"; break;
        default:
            if (static_cast<unsigned char>(c) < 0x20) {
                output += QString("\\u%1").arg(static_cast<unsigned char>(c), 4, 16, QLatin1Char('0')).toUtf8();
            } else {
                output += c;
            }
            break;
        }
    }
    return output;
}

} // namespace FitCore
