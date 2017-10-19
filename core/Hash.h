#ifndef HASH_H
#define HASH_H

#include <QObject>
#include <QJsonObject>

class Hash : public QObject
{
    Q_OBJECT
    
public:
    Hash();
    Hash(QByteArray hash);
    ///
    /// \brief Hash
    /// \param hash hex-encoded hash
    ///
    Hash(QString hash);
    ///
    /// \brief Hash
    /// \param hash hex-encoded hash
    ///
    Hash(const char *hash);
    Hash(const Hash &other);
        
    bool isNull() { return _hash.isEmpty(); }
    bool isHashOfEmpty();
    
    static Hash randomHash();
    
    QString toString() { return QString::fromUtf8(_hash.toHex()); }
    
    static Hash hash(const QJsonObject &object);
    
    bool operator ==(const Hash &other);
    bool operator ==(QByteArray hash);
    bool operator ==(QString hash);
    
    bool operator !=(const Hash &other);
    bool operator !=(QByteArray hash);
    bool operator !=(QString hash);
    
    bool operator >(const Hash &other);
    bool operator >(QByteArray hash);
    bool operator >(QString hash);
    
    bool operator <(const Hash &other);
    bool operator <(QByteArray hash);
    bool operator <(QString hash);
    
    void operator =(const Hash &other);
    void operator =(QByteArray hash);
    void operator =(QString hash);
    
private:
    QByteArray _hash;    
};

Q_DECLARE_METATYPE(Hash)

#endif // HASH_H
