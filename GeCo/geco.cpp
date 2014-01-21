#include "geco.h"

GeCo::GeCo()
{

}

QString GeCo::ucfirst(QString str)
{
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

QString GeCo::lcFirst(QString str)
{
    if (str.size() < 1) {
        return "";
    }

    QStringList tokens = str.split("_");
    QList<QString>::iterator tokItr = tokens.begin();

    for (tokItr = tokens.begin(); tokItr != tokens.end(); ++tokItr) {
        (*tokItr) = (*tokItr).at(0).toLower() + (*tokItr).mid(1);
    }
    return tokens.join("");
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
    this->beans.empty();

    QDomElement root = document.firstChildElement();

    QDomNodeList schemas = root.elementsByTagName("schema");
    for(int i = 0; i < schemas.count(); i++)
    {
        QDomNode schemaNode = schemas.at(i);

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

            for(int i = 0; i < relationsTables.count(); i++)
            {
                QDomNode relationTableName = relationsTables.at(i);

                if(relationTableName.isElement())
                {
                    QDomElement relationTableNameElement = relationTableName.toElement();
                    bean.addRelation(relationTableNameElement.attribute("value"));                  
                }
            }


            this->beans.append(bean);

        }
    }
}

void GeCo::generateCode(QSettings *settings)
{
    this->settings = settings;
    this->settings->beginGroup("parameters");
    this->path = this->settings->value("pathToSave").toString();
    this->settings->endGroup();
    this->createStructure();
    GeCoBean model;
    foreach (model, this->beans) {
        if(model.getModel())
            this->generateModel(model);
        if(model.getCrud())
            this->generateCRUD(model);

    }
   // qDebug() << this->beans;
}

void GeCo::generateModel(GeCoBean model)
{
    //Generate BEAN
    ZfBean *bean = new ZfBean(model,this->beans);
    bean->generate();
    if(!model.isDefaultModule())
        bean->write(this->path + "/module/" + model.getModule() + "/src/" + model.getModule() + "/Model/Bean/" + model.getName() + ".php");
    else
        bean->write(this->path + "/module/Application/src/Application/Model/Bean/" + model.getName() + ".php");

    //Generate Exception
    ZfException *exception = new ZfException(model,this->beans);
    exception->generate();
    if(!model.isDefaultModule())
        exception->write(this->path + "/module/" + model.getModule() + "/src/" + model.getModule() + "/Model/Exception/" + model.getName() + "Exception.php");
    else
        exception->write(this->path + "/module/Application/src/Application/Model/Exception/" + model.getName() + "Exception.php");

    //Generate Factory
    ZfFactory *factory = new ZfFactory(model,this->beans);
    factory->generate();
    if(!model.isDefaultModule())
        factory->write(this->path + "/module/" + model.getModule() + "/src/" + model.getModule() + "/Model/Factory/" + model.getName() + "Factory.php");
    else
        factory->write(this->path + "/module/Application/src/Application/Model/Factory/" + model.getName() + "Factory.php");

    //Generate Metadata
    ZfMetadata *metadata = new ZfMetadata(model,this->beans);
    metadata->generate();
    if(!model.isDefaultModule())
        metadata->write(this->path + "/module/" + model.getModule() + "/src/" + model.getModule() + "/Model/Metadata/" + model.getName() + "Metadata.php");
    else
        metadata->write(this->path + "/module/Application/src/Application/Model/Metadata/" + model.getName() + "Metadata.php");

    //Generate Collection
    ZfCollection *collection = new ZfCollection(model,this->beans);
    collection->generate();
    if(!model.isDefaultModule())
        collection->write(this->path + "/module/" + model.getModule() + "/src/" + model.getModule() + "/Model/Collection/" + model.getName() + "Collection.php");
    else
        collection->write(this->path + "/module/Application/src/Application/Model/Collection/" + model.getName() + "Collection.php");

    //Generate Catalog
    this->settings->beginGroup("parameters");
    QString prefixes = this->settings->value("prefixes").toString();

    this->settings->endGroup();
    ZfCatalog *catalog = new ZfCatalog(model,this->beans);
    catalog->generate(prefixes);
    if(!model.isDefaultModule())
        catalog->write(this->path + "/module/" + model.getModule() + "/src/" + model.getModule() + "/Model/Catalog/" + model.getName() + "Catalog.php");
    else
        catalog->write(this->path + "/module/Application/src/Application/Model/Catalog/" + model.getName() + "Catalog.php");

    //Generate Query
    ZfQuery *query = new ZfQuery(model,this->beans);
    query->generate(prefixes);
    if(!model.isDefaultModule())
        query->write(this->path + "/module/" + model.getModule() + "/src/" + model.getModule() + "/Query/" + model.getName() + "Query.php");
    else
        query->write(this->path + "/module/Application/src/Application/Query/" + model.getName() + "Query.php");


}

void GeCo::generateCRUD(GeCoBean model)
{
    QRegExp exp("([A-Z])");
    ZfCRUD *crud = new ZfCRUD(model, this->beans);
    crud->generate();
    if(!model.isDefaultModule())
        crud->write(this->path + "/module/" + model.getModule() + "/src/" + model.getModule() + "/Controller/" + model.getName() + "Controller.php");
    else
        crud->write(this->path + "/module/Application/src/Application/Controller/" + model.getName() + "Controller.php");

    QString viewModulePath = this->lcFirst(this->ucfirst(model.getModule())).replace(exp, "-\\1").toLower();

    QDir viewDirectory(this->path + "/module/Application/view/" + viewModulePath);
    if(!model.isDefaultModule())
        viewDirectory.setPath(this->path + "/module/" + model.getModule() + "/view/" + viewModulePath);


    if (!viewDirectory.exists())
        viewDirectory.mkdir(viewDirectory.path());


    ZfView *view = new ZfView(model, this->beans);
//    view->generate();
    if(!model.isDefaultModule())
        view->writeIndex(this->path + "/module/" + model.getModule() + "/view/"  + viewModulePath);
    else
        view->writeIndex(this->path + "/module/Application/src/Application/view/application");

    if(!model.isDefaultModule())
        view->writeForm(this->path + "/module/" + model.getModule() + "/view/" + viewModulePath);
    else
        view->writeForm(this->path + "/module/Application/src/Application/view/application");

    if(!model.isDefaultModule())
        view->writeHistory(this->path + "/module/" + model.getModule() + "/view/" + viewModulePath);
    else
        view->writeHistory(this->path + "/module/Application/src/Application/view/application");

}

void GeCo::createStructure()
{
    //Root Directory
    QDir dir(this->path);

    QDir modulePath(dir.path().append("/module"));
    if (!modulePath.exists())
        modulePath.mkdir(modulePath.path());

    QDir defaultModule(modulePath.path().append("/Application"));


    QDir src("src");
    QDir view("view");


    QDir controller("Controller");
    QDir model("Model");
    QDir query("Query");

    QDir bean("Bean");
    QDir catalog("Catalog");
    QDir collection("Collection");
    QDir exception("Exception");
    QDir factory("Factory");
    QDir metadata("Metadata");

QRegExp exp("([A-Z])");
    GeCoBean beanModel;
    foreach (beanModel, this->beans) {
        if(!beanModel.isDefaultModule())
        {

            QDir module(modulePath.path().append("/").append(beanModel.getModule()));
            QDir currentModule(modulePath.path().append("/").append(beanModel.getModule()));
            if (!module.exists())
                module.mkdir(module.path());

            module.setPath(currentModule.path().append("/").append(src.path()));

            QDir moduleSrc(module.path().append("/").append(beanModel.getModule()));
            QDir tmp(moduleSrc.path());

            if (!module.exists())
                module.mkdir(module.path());
            module.setPath(currentModule.path().append("/").append(view.path()));

            if (!module.exists())
                module.mkdir(module.path());//this->lcFirst(this->ucfirst(this->model.getName())).replace(exp, "-\\1").toLower()

            QString viewController = this->ucfirst(beanModel.getModule());
            QString viewControllerSepared = this->lcFirst(viewController);
            module.setPath(currentModule.path().append("/").append(view.path()).append("/").append(viewControllerSepared.replace(exp, "-\\1").toLower()));
            if (!module.exists())
                module.mkdir(module.path());

            if (!moduleSrc.exists())
                moduleSrc.mkdir(moduleSrc.path());

            tmp.setPath(tmp.path().append("/").append(controller.path()));
            if (!tmp.exists())
                tmp.mkdir(tmp.path());

            tmp.setPath(moduleSrc.path());
            tmp.setPath(tmp.path().append("/").append(model.path()));
            if (!tmp.exists())
                tmp.mkdir(tmp.path());

            tmp.setPath(moduleSrc.path());
            tmp.setPath(tmp.path().append("/").append(query.path()));
            if (!tmp.exists())
                tmp.mkdir(tmp.path());

            //MODEL
            tmp.setPath(moduleSrc.path());
            tmp.setPath(tmp.path().append("/").append(model.path()));
            tmp.setPath(tmp.path().append("/").append(bean.path()));
            if (!tmp.exists())
                tmp.mkdir(tmp.path());

            tmp.setPath(moduleSrc.path());
            tmp.setPath(tmp.path().append("/").append(model.path()));
            tmp.setPath(tmp.path().append("/").append(catalog.path()));
            if (!tmp.exists())
                tmp.mkdir(tmp.path());

            tmp.setPath(moduleSrc.path());
            tmp.setPath(tmp.path().append("/").append(model.path()));
            tmp.setPath(tmp.path().append("/").append(collection.path()));
            if (!tmp.exists())
                tmp.mkdir(tmp.path());

            tmp.setPath(moduleSrc.path());
            tmp.setPath(tmp.path().append("/").append(model.path()));
            tmp.setPath(tmp.path().append("/").append(exception.path()));
            if (!tmp.exists())
                tmp.mkdir(tmp.path());

            tmp.setPath(moduleSrc.path());
            tmp.setPath(tmp.path().append("/").append(model.path()));
            tmp.setPath(tmp.path().append("/").append(factory.path()));
            if (!tmp.exists())
                tmp.mkdir(tmp.path());

            tmp.setPath(moduleSrc.path());
            tmp.setPath(tmp.path().append("/").append(model.path()));
            tmp.setPath(tmp.path().append("/").append(metadata.path()));
            if (!tmp.exists())
                tmp.mkdir(tmp.path());
//            qDebug() << beanModel.getModule();
        }else{
            if (!defaultModule.exists())
                defaultModule.mkdir(defaultModule.path());

            QDir module(defaultModule.path());

            module.setPath(defaultModule.path().append("/").append(src.path()));

            QDir moduleSrc(module.path().append("/").append("Application"));
            QDir tmp(moduleSrc.path());
            if (!module.exists())
                module.mkdir(module.path());
            module.setPath(defaultModule.path().append("/").append(view.path()));

            if (!module.exists())
                module.mkdir(module.path());


            if (!moduleSrc.exists())
                moduleSrc.mkdir(moduleSrc.path());

            tmp.setPath(tmp.path().append("/").append(controller.path()));
            if (!tmp.exists())
                tmp.mkdir(tmp.path());

            tmp.setPath(moduleSrc.path());
            tmp.setPath(tmp.path().append("/").append(model.path()));
            if (!tmp.exists())
                tmp.mkdir(tmp.path());

            tmp.setPath(moduleSrc.path());
            tmp.setPath(tmp.path().append("/").append(query.path()));
            if (!tmp.exists())
                tmp.mkdir(tmp.path());

            //MODEL
            tmp.setPath(moduleSrc.path());
            tmp.setPath(tmp.path().append("/").append(model.path()));
            tmp.setPath(tmp.path().append("/").append(bean.path()));
            if (!tmp.exists())
                tmp.mkdir(tmp.path());

            tmp.setPath(moduleSrc.path());
            tmp.setPath(tmp.path().append("/").append(model.path()));
            tmp.setPath(tmp.path().append("/").append(catalog.path()));
            if (!tmp.exists())
                tmp.mkdir(tmp.path());

            tmp.setPath(moduleSrc.path());
            tmp.setPath(tmp.path().append("/").append(model.path()));
            tmp.setPath(tmp.path().append("/").append(collection.path()));
            if (!tmp.exists())
                tmp.mkdir(tmp.path());

            tmp.setPath(moduleSrc.path());
            tmp.setPath(tmp.path().append("/").append(model.path()));
            tmp.setPath(tmp.path().append("/").append(exception.path()));
            if (!tmp.exists())
                tmp.mkdir(tmp.path());

            tmp.setPath(moduleSrc.path());
            tmp.setPath(tmp.path().append("/").append(model.path()));
            tmp.setPath(tmp.path().append("/").append(factory.path()));
            if (!tmp.exists())
                tmp.mkdir(tmp.path());

            tmp.setPath(moduleSrc.path());
            tmp.setPath(tmp.path().append("/").append(model.path()));
            tmp.setPath(tmp.path().append("/").append(metadata.path()));
            if (!tmp.exists())
                tmp.mkdir(tmp.path());

        }
    }

}
