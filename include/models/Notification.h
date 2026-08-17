#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <QString>

namespace FitCore {

class Notification {
public:
    Notification();

    int getId() const;
    void setId(int id);

    int getUserId() const;
    void setUserId(int userId);

    QString getTitle() const;
    void setTitle(const QString& title);

    QString getMessage() const;
    void setMessage(const QString& message);

    QString getType() const;
    void setType(const QString& type);

    QString getCreatedAt() const;
    void setCreatedAt(const QString& createdAt);

    QString getReadAt() const;
    void setReadAt(const QString& readAt);

    bool isRead() const;
    void setRead(bool isRead);

private:
    int m_id;
    int m_userId;
    QString m_title;
    QString m_message;
    QString m_type;
    QString m_createdAt;
    QString m_readAt;
    bool m_isRead;
};

} // namespace FitCore

#endif // NOTIFICATION_H
