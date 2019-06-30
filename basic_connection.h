#pragma base
#include <QtCore/QString>

class ConnectionFactory;

class BasicConnection {
public:
    void setAccount(QString s) { m_acc = s; }
    const QString& account() const { return m_acc; }

    void connect();
protected:
    virtual ConnectionFactory& factory() = 0;
private:
    QString m_acc;
};