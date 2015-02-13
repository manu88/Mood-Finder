#ifndef DATAMODEL_H
#define DATAMODEL_H
#include <QList>
#include <QAbstractTableModel>

struct Timecode
{
    Timecode():
        hours   ( 0 ),
        minutes ( 0 ),
        seconds ( 0 ),
        millis  ( 0 )
    {}
    unsigned int hours;
    unsigned int minutes;
    unsigned int seconds;
    unsigned int millis;

    bool operator ==( const Timecode &rhs)
    {
        return ( hours   == rhs.hours   ) &&
               ( minutes == rhs.minutes ) &&
               ( seconds == rhs.seconds ) &&
               ( millis  == rhs.millis  );
    }

    static QString tcToQString(const Timecode &tc)
    {
        return QString("%1:%2:%3:%4").arg(tc.hours)
                                     .arg(tc.minutes)
                                     .arg(tc.seconds)
                                     .arg(tc.millis);
    }

} ;

class DataEntry
{
public:
    DataEntry()
    {
    }


    ~DataEntry()
    {

    }

    QString  projectName;
    QString  description;
    QString  filePath;
    QString  remarques;
    Timecode timecode;

    bool operator ==(const DataEntry &rhs)
    {
        return ( projectName == rhs.projectName ) &&
               ( description == rhs.description ) &&
               ( filePath    == rhs.filePath    ) &&
               ( remarques   == rhs.remarques   ) &&
               ( timecode    == rhs.timecode    );
    }

};

class DataModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    DataModel( QObject *parent );
    ~DataModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    Qt::ItemFlags flags(const QModelIndex & index) const ;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    void parseProjectDirectory(const QString &dirPath);


    void addEntry( const DataEntry &entry);


    void search( const QString &filter);

    /* XML stuffs */

    bool loadXML( const QString &fromFile);

    bool compareAndSaveXML( const QString &toFile);

signals:

    void editCell( int row , int col, const QString &value);

private slots:

    void didEditCell( int row , int col, const QString &value);

private:
    // return true if done something in _dataList
    bool commitDataList( const QList<DataEntry> &parseList);

    QList< DataEntry > _dataList;

};

#endif // DATAMODEL_H
