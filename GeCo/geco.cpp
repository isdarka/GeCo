#include "geco.h"

GeCo::GeCo()
{

}

QString GeCo::camelCase(QString str) {

    QString prefix;
    foreach (prefix, this->prefixes) {
        str.replace(prefix,"");
    }
    if (str.size() < 1) {
        return "";
    }

    QStringList tokens = str.split("_");
    QList<QString>::iterator tokItr = tokens.begin();

    for (tokItr = tokens.begin(); tokItr != tokens.end(); ++tokItr) {
        (*tokItr) = (*tokItr).at(0).toUpper() + (*tokItr).mid(1);
    }
    return tokens.join("");
}

void GeCo::generateXML(QSettings *settings)
{
    try{
        QString schema;
        settings->beginGroup("parameters");

        this->path = settings->value("pathToSave").toString();
        this->prefixes = settings->value("prefixes").toString().split(",");
        settings->endGroup();
        settings->beginGroup("MysqlSettings");
        schema = settings->value("schema").toString();
        settings->endGroup();

       this->write(this->getDefault());
    }catch(...)
    {
        throw QString("Error al generar");
    }
}

QDomDocument GeCo::getDefault()
{
    QDomDocument document;
    QDomElement root = document.createElement("GeCo");
    QSqlQuery query;
    //Add it to the document
    document.appendChild(root);
    query.exec("SHOW TABLES");

    QString tablename;
    while(query.next()) {

        QDomElement schema = document.createElement("schema");

        root.appendChild(schema);
        tablename = query.value(0).toString();

        QDomElement name = document.createElement("name");

            name.setAttribute("value", this->camelCase(query.value(0).toString()));



        schema.appendChild(name);
        QDomElement table = document.createElement("table");
        table.setAttribute("value", query.value(0).toString());
        schema.appendChild(table);
        QDomElement module = document.createElement("module");
        module.setAttribute("value", "default");
        schema.appendChild(module);
        QDomElement extend = document.createElement("extend");
        extend.setAttribute("value", "");
        schema.appendChild(extend);
        QDomElement relations = document.createElement("relations");
        schema.appendChild(relations);
            QDomElement rel_table = document.createElement("table");
            rel_table.setAttribute("value", "");
            relations.appendChild(rel_table);


        QDomElement model = document.createElement("model");
        model.setAttribute("value", true);
        schema.appendChild(model);
        QDomElement controller = document.createElement("crud");
        controller.setAttribute("value", true);
        schema.appendChild(controller);
        QDomElement form = document.createElement("form");
        form.setAttribute("value", true);
        schema.appendChild(form);
        QDomElement view = document.createElement("view");
        view.setAttribute("value", true);
        schema.appendChild(view);

        QDomElement log = document.createElement("log");
        log.setAttribute("enable", false);
        log.setAttribute("tableName", "tableName");
        schema.appendChild(log);
    }
    return document;
}

void GeCo::write(QDomDocument document)
{
    QFile file(this->path.append("/Default.xml"));
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        throw QString("Failed to open file for writting");
    }
    else
    {
        QTextStream stream(&file);
        stream << document.toString();
        file.close();
    }
}


void GeCo::read(QDomDocument document)
{
    QVector<GeCoBean> beans;
    //get the root element
    QDomElement root = document.firstChildElement();

    //List the books
//    ListElements(root,"Schema", "Name");

  //  qDebug() << "\r\nMore Advanced\r\n";

    //Get the chapters
    QDomNodeList schemas = root.elementsByTagName("schema");
  //  qDebug() << schemas.count();
    for(int i = 0; i < schemas.count(); i++)
    {
        QDomNode schemaNode = schemas.at(i);
       // qDebug() << schemas.QDomNodeList().count();

        //convert to an element
        if(schemaNode.isElement())
        {
           GeCoBean bean;
           QDomElement schema = schemaNode.toElement();

           QDomNode name = schema.elementsByTagName("name").at(0);
           QDomNode table = schema.elementsByTagName("table").at(0);
           QDomNode module = schema.elementsByTagName("module").at(0);
           QDomNode extend = schema.elementsByTagName("extend").at(0);
           QDomNode model = schema.elementsByTagName("model").at(0);
           QDomNode crud = schema.elementsByTagName("crud").at(0);
           QDomNode form = schema.elementsByTagName("form").at(0);
           QDomNode log = schema.elementsByTagName("log").at(0);

           QDomNode relationsNode = schema.elementsByTagName("relations").at(0);
           QDomElement relations = relationsNode.toElement();
           QDomNodeList relationsTables = relations.elementsByTagName("table");

           bean.setName(name.toElement().attribute("value"));
           bean.setTable(table.toElement().attribute("value"));
           bean.setModule(module.toElement().attribute("value"));
           bean.setExtend(extend.toElement().attribute("value"));
           bean.setModel(model.toElement().attribute("value").toInt());
           bean.setCrud(crud.toElement().attribute("value").toInt());
           bean.setForm(form.toElement().attribute("value").toInt());
           bean.setLog(log.toElement().attribute("enable").toInt());
           bean.setLogName(log.toElement().attribute("tableName"));


           qDebug() <<name.toElement().attribute("value");
           qDebug() <<table.toElement().attribute("value");
           qDebug() <<module.toElement().attribute("value");
           qDebug() <<extend.toElement().attribute("value");
           qDebug() <<model.toElement().attribute("value");
           qDebug() <<crud.toElement().attribute("value");
           qDebug() <<form.toElement().attribute("value");
           qDebug() <<log.toElement().attribute("tableName");
            for(int i = 0; i < relationsTables.count(); i++)
            {
                QDomNode relationTableName = relationsTables.at(i);
                //convert to an element
                if(relationTableName.isElement())
                {
                    QDomElement relationTableNameElement = relationTableName.toElement();
                    bean.addRelation(relationTableNameElement.attribute("value"));
                   // qDebug() << "Chapters in " << relationTableNameElement.attribute("value");

                }
            }
            //qDebug() << "Relations = " << beans;


            beans.append(bean);
           /*
 <name value="Address"/>
  <table value="common_address"/>
  <module value="default"/>
  <extend value=""/>
  <relations>
   <table value=""/>
  </relations>
  <model value="1"/>
  <controller value="1"/>
  <form value="1"/>
  <view value="1"/>
  <log tableName="tableName" enable="0"/>

  */
//           qDebug() <<name.toElement().attribute("value");
//           qDebug() <<table.toElement().attribute("value");
//           qDebug() <<module.toElement().attribute("value");
//           qDebug() <<extend.toElement().attribute("value");
//           qDebug() <<model.toElement().attribute("value");
//           qDebug() <<controller.toElement().attribute("value");
//           qDebug() <<form.toElement().attribute("value");
//           qDebug() <<log.toElement().attribute("tableName");
        /*   QDomNodeList items = schema.elementsByTagName("name");
           qDebug() << "Total items = " << items.count();
           QDomNode name = items.at(i);
           if(name.isElement())
           {
               QDomElement itemele = name.toElement();
               qDebug() << itemele.attribute("value");
           }*/
//            QDomNodeList name = params.elementsByTagName("name");
            //qDebug() << name..toElement().attribute("value");

          //  qDebug() << "Chapters in " << book.attribute("value");
            //ListElements(book,"Chapter","Name");
        }
    }

  //  qDebug() << "Finished";
}
