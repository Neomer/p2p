#ifndef HASH_H
#define HASH_H

#include <QObject>

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

#endif // HASH_H
