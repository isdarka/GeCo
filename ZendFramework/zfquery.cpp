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

void ZfQuery::generate(QString prefixString)
{
    QStringList prefixes = prefixString.split(",");
    GeCoBean extra;
    this->code.setClassName(this->model.getName() + "Query");
    if(!this->model.getExtend().isEmpty())
        extra = this->getByExntendName(this->model.getExtend());

    this->code.setExtends("Query");
    this->code.addUse("Query\\Query");
    this->code.addUse(this->model.getModule() + "\\Model\\Metadata\\" + this->model.getName() + "Metadata");





        //RELATIONS
    QRegExp exp("([A-Z])");
    QString relation;
    foreach (relation, this->model.getRelations()) {
        if(!relation.isEmpty())
        {
            QString tmp = relation;
            relation.replace(this->model.getName().replace(exp, "_\\1").append("s").toLower(), "");
            relation.replace(this->model.getName().replace(exp, "_\\1").toLower(), "");
            relation.replace(this->model.getModule().toLower(), "");
            QString prefix;
            foreach (prefix, prefixes) {
                relation.replace(prefix, "", Qt::CaseInsensitive);
            }

            relation.replace("_s", "");
            relation.replace("s_", "");
            relation.replace("_", "");
            relation = relation.mid(0, relation.size() - 1);

            Method methodJoin;
            Docblock docblockJoin;
            docblockJoin.setShortDescription("Inner Join " + this->ucfirst(relation));
            methodJoin.setName("innerJoin"+ this->ucfirst(relation));
            methodJoin.setVisibility(Method::PUBLIC);
            methodJoin.isStatic(false);
            methodJoin.addBody("$this->join(\""+ tmp +"\",");
            methodJoin.addBody("\t\""+ tmp +".\".$this->metadata->getPrimaryKey().\"=\".");
            methodJoin.addBody("\t$this->metadata->getEntityName().\".\".$this->metadata->getPrimaryKey());");
            methodJoin.addBody("return $this;");
            methodJoin.setDocblock(docblockJoin);
            this->code.addMethod(methodJoin);
        }
    }
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

    QString methodDocblock;
    methodDocblock = "\\" + this->model.getModule() + "\\Model\\Collection\\" + this->model.getName() + "Collection find()";
    this->code.addMethodsDocblock(methodDocblock);
    methodDocblock = "\\" + this->model.getModule() + "\\Model\\Bean\\" + this->model.getName() + " findByPK() findByPK($primaryKey)";
    this->code.addMethodsDocblock(methodDocblock);
    methodDocblock = "\\" + this->model.getModule() + "\\Model\\Bean\\" + this->model.getName() + " findByPKOrThrow() findByPKOrThrow($primaryKey, $exception)";
    this->code.addMethodsDocblock(methodDocblock);
    methodDocblock = "\\" + this->model.getModule() + "\\Model\\Bean\\" + this->model.getName() + " findOne()";
    this->code.addMethodsDocblock(methodDocblock);


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
