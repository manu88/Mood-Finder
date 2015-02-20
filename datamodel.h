#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <QList>
#include <QAbstractTableModel>
#include <QDate>

struct Timecode
{
    Timecode():
        hours   ( 0 ),
        minutes ( 0 ),
        seconds ( 0 ),
        millis  ( 0 )
    {}

    Timecode( const QString &str)
    {
        auto list = str.split(":");

        if (list.size() == 4)
        {
            hours   = list[0].toInt();
            minutes = list[1].toInt();
            seconds = list[2].toInt();
            millis  = list[3].toInt();
        }

        validate();
    }

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

    void validate()
    {
        int add = millis / 1000.;
        millis %= 1000;

        seconds+=add;

        add = seconds/ 60. ;
        seconds %= 60;

        minutes+=add;

        add = minutes / 60. ;
        minutes %= 60;

        hours += add;

        hours %= 24;
    }

} ;

/* **** **** **** **** **** **** **** **** */

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

    //int     uniqueID;

    bool operator ==(const DataEntry &rhs)
    {
        return ( projectName == rhs.projectName ) &&
               ( description == rhs.description ) &&
               ( filePath    == rhs.filePath    ) &&
               ( remarques   == rhs.remarques   ) &&
               ( timecode    == rhs.timecode    );
    }

    bool contains( const QString &srch) const
    {
        return    description.contains( srch , Qt::CaseInsensitive)
               || projectName.contains( srch , Qt::CaseInsensitive)
               || filePath.contains( srch , Qt::CaseInsensitive)
               || remarques.contains( srch , Qt::CaseInsensitive);
    }

};

/* **** **** **** **** **** **** **** **** */

typedef enum
{
    DataListMode = 0,
    DataSearchMode = 1
} DataPresentationMode;

class DataModel : public QAbstractTableModel
{
    Q_OBJECT
public:


    DataModel( QObject *parent );
    ~DataModel();

    /* QAbstractTableModel reimp */

    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    Qt::ItemFlags flags(const QModelIndex & index) const ;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    /**/

    const QDateTime &getDataBaseDate() const
    {
        return _date;
    }

    DataPresentationMode getCurrentMode() const
    {
        return _currentMode;
    }

    void ChangePresentationModeTo( DataPresentationMode mode );


    /**/

    void parseProjectDirectory(const QString &dirPath);


    void addEntry( const DataEntry &entry);
    void duplicateEntryNum( int num);
    void deleteEntryNum( int num);


    int search( const QString &filter);
    void clearSearch();

    /* XML stuffs */

    bool loadXML( const QString &fromFile);

    bool compareAndSaveXML( const QString &toFile);

    const DataEntry &getEntryForIndex( int index)
    {
        return _dataList.at(index);
    }

signals:

    void editCell( int row , int col, const QString &value);

private slots:

    void didEditCell( int row , int col, const QString &value);

private:
    // return true if done something in _dataList
    bool commitDataList( const QList<DataEntry> &parseList);

    void commitChangesInSearchList();

    // pointer to _dataList if _currentMode == DataListMode
    // or _dataSearchList if _currentMode == DataSearchMode
    QList< DataEntry> *_currentList;
    QList< DataEntry > _dataList;
    QList< DataEntry > _dataSearchList;

    bool _editedInSearchMode;

    QDateTime _date;

    DataPresentationMode _currentMode;

};

#endif // DATAMODEL_H
