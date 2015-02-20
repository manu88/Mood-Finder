
#include <QDebug>
#include <QDomDocument>
#include <QXmlStreamWriter>
#include <QFile>
#include <QDir>


#include "commons.h"
#include "datamodel.h"


DataModel::DataModel(QObject *parent) :
    _editedInSearchMode ( false ),
    _currentMode        ( DataListMode )
{
    _currentList = &_dataList;
    connect(this , SIGNAL(editCell(int,int,QString)) , this , SLOT(didEditCell(int,int,QString)) );
}

DataModel::~DataModel()
{

}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

int DataModel::rowCount(const QModelIndex &parent) const
{
    return _currentList->size();
}
int DataModel::columnCount(const QModelIndex &parent) const
{
    return 5;
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

QVariant DataModel::data(const QModelIndex &index, int role ) const
{


    if ((role == Qt::DisplayRole) || (role == Qt::EditRole  ) )
    {
        const DataEntry & entry = _currentList->at(index.row());

        switch (index.column())
        {
            case 0: //desc
                return entry.description;
            case 1: // file
                return entry.filePath;
            case 2: // tc
                return Timecode::tcToQString(entry.timecode);

            case 3: // remarques
                return entry.remarques;
            case 4:
                return entry.projectName ;
        }

     }

     return QVariant();
}
/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

Qt::ItemFlags DataModel::flags(const QModelIndex & index) const
{
     return Qt::ItemIsSelectable |  Qt::ItemIsEditable | Qt::ItemIsEnabled ;
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

bool DataModel::setData(const QModelIndex & index, const QVariant & value, int role )
{
    if (role == Qt::EditRole)
    {

        emit editCell( index.row() , index.column() , value.toString() );

    }
    return true;
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

void DataModel::didEditCell( int row , int col, const QString &value)
{

    if (_currentMode == DataSearchMode)
        _editedInSearchMode = true;

    DataEntry &entry = (*_currentList)[ row ];

    switch ( col )
    {
    case 0: //Description
        entry.description = value;

        break;

    case 1: //Fichier

        break;
    case 2: //Timecode
        entry.timecode = Timecode(value);
        break;
    case 3: // Remarques
        entry.remarques = value;

        break;
    case 4: //Projet

        break;
    }


}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

QVariant DataModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
         {
             if (orientation == Qt::Horizontal)
             {
                 switch (section)
                 {
                 case 0:
                     return QString("Description");
                 case 1:
                     return QString("Fichier");
                 case 2:
                     return QString("Timecode");
                 case 3:
                     return QString("Remarques");
                 case 4:
                     return QString("Projet");
                 }
             }

             else if (orientation == Qt::Vertical)
             {
                 return QString("%1").arg(section );
             }
         }
         return QVariant();

}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

void DataModel::ChangePresentationModeTo( DataPresentationMode mode )
{
    if (mode != _currentMode)
    {
        // emit change in data
        beginResetModel();

        _currentMode = mode;

        // back to the whole list
        if ( _currentMode == DataListMode)
        {
            _currentList = &_dataList;

            // commit changes from __dataSearchList to _dataList, if any
            if (_editedInSearchMode)
                commitChangesInSearchList();

            _editedInSearchMode = false;
        }

        // search list
        else if ( _currentMode == DataSearchMode)
        {
            _currentList = &_dataSearchList;
        }

        endResetModel();
    }
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

void DataModel::parseProjectDirectory( const QString &dirPath)
{
    QList< DataEntry > tempData;
    QDir dir( dirPath );

    if ( !dir.isReadable())
    {
        qDebug("Error : project Dir is not readable ...");

        return;
    }

    // so far so good

    qDebug("Begin parse in %s", dir.absolutePath().toStdString().c_str());

    QStringList projectList = dir.entryList();

    //startEditDataList();

    foreach (const QString &proj, projectList)
    {
        if ( (proj != ".") && (proj != "..") )
        {
            //qDebug("got a dir %s", proj.toStdString().c_str());
            QDir media;
            media.setPath(dir.absolutePath() +"/" + proj + "/" + DIR_RUSHES_NAME);

            if ( !media.exists() )
                media.setPath(dir.absolutePath() +"/" + proj + "/" + DIR_RUSHES_NAME2);

            if ( !media.exists() )
                media.setPath(dir.absolutePath() +"/" + proj + "/" + DIR_RUSHES_NAME3);

            if ( !media.exists() )
                media.setPath(dir.absolutePath() +"/" + proj + "/" + DIR_RUSHES_NAME4);

            if ( !media.exists() )
                media.setPath(dir.absolutePath() +"/" + proj + "/" + DIR_RUSHES_NAME5);

            if ( !media.exists() )
                media.setPath(dir.absolutePath() +"/" + proj + "/" + DIR_RUSHES_NAME6);

            if ( !media.exists() )
                media.setPath(dir.absolutePath() +"/" + proj + "/" + DIR_RUSHES_NAME7);

            if ( media.exists() )
            {
                //qDebug("found rushes file for %s", proj.toStdString().c_str());

                QFileInfoList vidList = media.entryInfoList(QDir::Files , QDir::Name );

                foreach ( const QFileInfo &file, vidList)
                {
                    //qDebug("Got a rush %s", file.fileName().toStdString().c_str());

                    DataEntry entry;
                    entry.projectName = proj;
                    entry.filePath = file.fileName();

                    tempData.push_back( entry);
                    //addEntry( entry );
                }



            }
            else
                qDebug("DID NOT found rushes file for %s", proj.toStdString().c_str());

        }

    }

    commitDataList( tempData );
    //endEditDataList();
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

bool DataModel::commitDataList( const QList<DataEntry> &parseList)
{
    bool comp = parseList == _dataList;

    if ( comp)
        qDebug("Lists are the same");
    else
    {

        qDebug("Lists are _NOT_ the same");

        beginResetModel();
        _dataList = parseList;
        endResetModel();


    }


    return false;
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

void DataModel::commitChangesInSearchList()
{

}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

void DataModel::addEntry( const DataEntry &entry)
{
    _dataList.push_back( entry );

    qDebug("entry added %i" ,_dataList.size() );
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
void DataModel::duplicateEntryNum( int num)
{
    const DataEntry source = _currentList->at(num);

    _currentList->insert( num+1 , DataEntry(source) );
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

void DataModel::deleteEntryNum( int num)
{
    _currentList->removeAt( num);
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

int DataModel::search( const QString &filter)
{


    _dataSearchList.clear();
    foreach (const DataEntry &entry, _dataList)
    {
        if ( entry.contains( filter) )
        {
            _dataSearchList.push_back( entry );
        }
    }

    ChangePresentationModeTo( DataSearchMode);

    return _dataSearchList.size();
}

void DataModel::clearSearch()
{
    ChangePresentationModeTo( DataListMode );
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

bool DataModel::loadXML( const QString &fromFile)
{
    QDomDocument xmlData;

    QFile f( fromFile );
    if (!f.open(QIODevice::ReadOnly ))
    {
        // Error while loading file
        qDebug("Error while loading file");
        return false;
    }
    // Set data into the QDomDocument before processing
    xmlData.setContent( &f );
    f.close();

    /* HEADER parse*/

    QDomNode header = xmlData.elementsByTagName(XML_TAG_HEADER).at(0);

    for ( QDomNode node = header.firstChild(); !node.isNull() ; node=node.nextSibling()  )
    {
        if( node.isElement() )
        {
            const QDomElement elem = node.toElement();
            qDebug() << "element " << elem.tagName() << " val " << elem.text();

            if (elem.tagName() == XML_TAG_SAVE_DATE)
                _date = QDateTime::fromString(elem.text() , Qt::ISODate);


        }

    }

    /* RUHSES list parse*/

    QDomNodeList list = xmlData.elementsByTagName( XML_TAG_RUSHES);
    qDebug() << "list size " << list.size();
    QDomNode rushes = list.at(0);

    beginResetModel();

    _dataList.clear();
    for ( QDomNode node = rushes.firstChild(); !node.isNull() ; node=node.nextSibling()  )
    {
        if (node.isElement() && (node.toElement().tagName()==XML_TAG_ENTRY) )
        {
            const QDomElement elem = node.toElement();

            const QDomNodeList attr = elem.childNodes();

            DataEntry entry;

            entry.filePath = node.toElement().tagName();



            for (int i =0; i< attr.size(); i++)
            {
                if (attr.at(i).isElement())
                {
                    const QDomElement elem = attr.at(i).toElement();

                    if ( elem.tagName() == XML_TAG_PATH)
                        entry.filePath = elem.text();

                    else if ( elem.tagName() == XML_TAG_PROJECT )
                        entry.projectName = elem.text();

                    else if ( elem.tagName() == XML_TAG_DESC )
                        entry.description = elem.text();

                    else if ( elem.tagName() == XML_TAG_REMARK )
                        entry.remarques = elem.text();

                    else if ( elem.tagName() == XML_TAG_TIMECODE)
                        entry.timecode = Timecode(elem.text() );
                }

            }

            _dataList.push_back( entry );



        }

        else
        {
            qDebug() << "Node is not Element";
        }


    }

    endResetModel();
    return true;
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

bool DataModel::compareAndSaveXML( const QString &toFile)
{
    /*
    QDomDocument oldXmlData;

    QFile f( toFile );
    if (!f.open(QIODevice::ReadOnly ))
    {
        // Error while loading file
        qDebug("Error while loading file");
        return false;
    }
    // Set data into the QDomDocument before processing
    oldXmlData.setContent( &f );
    f.close();


    QDomNodeList list = oldXmlData.elementsByTagName("RUSHES");
    QDomNode rushes = list.at(0);

    */
    QFile file( toFile );

    file.open(QIODevice::WriteOnly);
    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();

    xmlWriter.writeStartElement(XML_TAG_ROOT);

    xmlWriter.writeStartElement(XML_TAG_HEADER);
    xmlWriter.writeTextElement(XML_TAG_SAVE_DATE , QDateTime::currentDateTime().toString( Qt::ISODate) );
    xmlWriter.writeEndElement();

    xmlWriter.writeStartElement(XML_TAG_RUSHES);

    foreach (const DataEntry &entry, _dataList)
    {
        xmlWriter.writeStartElement(XML_TAG_ENTRY);
        xmlWriter.writeTextElement(XML_TAG_PROJECT, entry.projectName );
        xmlWriter.writeTextElement(XML_TAG_DESC,entry.description);
        xmlWriter.writeTextElement(XML_TAG_TIMECODE,Timecode::tcToQString(entry.timecode) );
        xmlWriter.writeTextElement(XML_TAG_REMARK,entry.remarques );
        xmlWriter.writeTextElement(XML_TAG_PATH,entry.filePath);
        xmlWriter.writeEndElement();
    }

    xmlWriter.writeEndElement(); /*XML_TAG_RUSHES*/

    xmlWriter.writeEndElement(); /*XML_TAG_ROOT*/

    xmlWriter.writeEndDocument();

    file.close();

    return true;
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
