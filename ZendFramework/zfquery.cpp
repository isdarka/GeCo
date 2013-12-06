#include "zfquery.h"

ZfQuery::ZfQuery(GeCoBean model, QVector<GeCoBean> models)
{
    this->model = model;
    this->models = models;

}



QString ZfQuery::ucfirst(QString str)
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

QString ZfQuery::lcFirst(QString str)
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

void ZfQuery::generate()
{

    GeCoBean extra;
    this->code.setClassName(this->model.getName() + "Query");
    if(!this->model.getExtend().isEmpty())
        extra = this->getByExntendName(this->model.getExtend());

    this->code.setExtends("Query");
    this->code.addUse("Query\\Query");
    this->code.addUse(this->model.getModule() + "\\Model\\Metadata\\" + this->model.getName() + "Metadata");


    Method methodConstruct;
    Docblock docblockConstruct;
    docblockConstruct.setShortDescription("Contruct " + this->model.getName() + "Query");
    methodConstruct.setName("__construct");
    methodConstruct.addParam("adapter");
    methodConstruct.setVisibility(Method::PUBLIC);
    methodConstruct.isStatic(false);
    methodConstruct.addBody("$this->metadata = new " + this->model.getName() + "Metadata();");
    methodConstruct.addBody("parent::__construct($adapter, $this->metadata->getTableName(), $this->metadata->getEntityName());");
    if(!this->model.getExtend().isEmpty())
    {
        methodConstruct.addBody("$this->innerJoin(new " + extra.getName() +"());");
        this->code.addUse(extra.getModule() + "\\Model\\Bean\\" + extra.getName());
    }


    methodConstruct.setDocblock(docblockConstruct);
    this->code.addMethod(methodConstruct);


    Docblock docblock;
    docblock.setShortDescription(this->model.getName() + "Query");
    docblock.setLongDescription("GeCo");
    docblock.addTag("autor", "isdarka");
    docblock.addTag("category", "Model");
    docblock.addTag("package", "Query");
    docblock.addTag("copyright", "");
    docblock.addTag("license", "");
    docblock.addTag("created", QDateTime::currentDateTime().toString());
    docblock.addTag("version", "1.0");

    this->code.setDocblock(docblock);

    this->code.setNamespace(this->model.getModule() + "\\Query");


}

GeCoBean ZfQuery::getByExntendName(QString extend)
{
    GeCoBean model;
    foreach (model, this->models) {
        if(model.getName() == extend)
            return model;
    }
}

void ZfQuery::write( QString str)
{
    QFile file(str);
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream out(&file);
        out << this->code.generate();

        // optional, as QFile destructor will already do it:
        file.close();
}
