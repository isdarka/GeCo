#include "zfexception.h"

ZfException::ZfException(GeCoBean model, QVector<GeCoBean> models)
{
    this->model = model;
    this->models = models;

}



QString ZfException::ucfirst(QString str)
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

QString ZfException::lcFirst(QString str)
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

void ZfException::generate()
{

    GeCoBean extra;
    this->code.setClassName(this->model.getName() + "Exception");
    if(!this->model.getExtend().isEmpty())
    {
        extra = this->getByExntendName(this->model.getExtend());
     //   qDebug() << extra.getModule();
        this->code.setExtends(extra.getName() + "Exception");
    }else
        this->code.setExtends("\\Exception");


    Docblock docblock;
    docblock.setShortDescription(this->model.getName() + "Bean");
    docblock.setLongDescription("GeCo");
    docblock.addTag("autor", "isdarka");
    docblock.addTag("category", "Model");
    docblock.addTag("package", "Exception");
    docblock.addTag("copyright", "");
    docblock.addTag("license", "");
    docblock.addTag("created", QDateTime::currentDateTime().toString());
    docblock.addTag("version", "1.0");

    this->code.setDocblock(docblock);

    this->code.setNamespace(this->model.getModule() + "\\Model\\Exception");
    if(!this->model.getExtend().isEmpty())
        if(this->model.getModule() != extra.getModule())
            this->code.addUse(extra.getModule() + "\\Model\\Exception\\" + extra.getName() + "Exception");


}

GeCoBean ZfException::getByExntendName(QString extend)
{
    GeCoBean model;
    foreach (model, this->models) {
        if(model.getName() == extend)
            return model;
    }
}

void ZfException::write( QString str)
{
    QFile file(str);
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream out(&file);
        out << this->code.generate();

        // optional, as QFile destructor will already do it:
        file.close();
}
