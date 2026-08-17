#include <QApplication>
#include <QPushButton>
#include <QSqlDatabase>
#include <QDebug>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QPushButton btn("Hello FitCore!");
    btn.resize(200, 100);
    btn.show();
    qDebug() << "Qt Version:" << QT_VERSION_STR;
    return 0;
}
