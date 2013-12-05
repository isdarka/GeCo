#include "zfcollection.h"

ZfCollection::ZfCollection(GeCoBean model, QVector<GeCoBean> models)
{
    this->model = model;
    this->models = models;

}
void ZfCollection::generate()
{
    GeCoBean extra;
    this->code.setClassName(this->model.getModule() + "_Model_Collection_" + this->model.getName());
    this->code.setDocblock(this->classDocBlock());

    if(!this->model.getExtend().isEmpty())
    {
        extra = this->getByExntendName(this->model.getExtend());
     //   qDebug() << extra.getModule();
        this->code.setExtends(extra.getModule() + "_Model_Collection_" + extra.getName());
    }

    this->code.addMethod(this->getConstruct());
    this->code.addMethod(this->getAppend());
}

QString ZfCollection::ucfirst(QString str)
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
GeCoBean ZfCollection::getByExntendName(QString extend)
{
    GeCoBean model;
    foreach (model, this->models) {
        if(model.getName() == extend)
            return model;
    }
}

void ZfCollection::write( QString str)
{
    QFile file(str);
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream out(&file);
        out << this->code.generate();

        // optional, as QFile destructor will already do it:
        file.close();
}

Docblock ZfCollection::classDocBlock()
{
    Docblock docblock;
    docblock.setShortDescription(this->model.getName() + "Bean");
    docblock.setLongDescription("Code Generator");
    docblock.addTag("autor", "");
    docblock.addTag("category", "Models");
    docblock.addTag("package", "Bean");
    docblock.addTag("copyright", "");
    docblock.addTag("license", "");
    docblock.addTag("created", QDateTime::currentDateTime().toString());
    docblock.addTag("version", "1.0");
    return docblock;
}

Method ZfCollection::getConstruct()
{
    Method construct;
    construct.setName("__construct");
    construct.setVisibility(Method::PUBLIC);
    construct.addParam("$array = array()","array");
    construct.addBody("parent::__construct($array);");
    Docblock docblock;
    docblock.setShortDescription("Construct an ArrayIterator");
    docblock.addTag("param", "array $array");
    construct.setDocblock(docblock);

    return construct;
}

Method ZfCollection::getAppend()
{
    Method append;
    Docblock docblock;
    append.setName("append");
    append.setVisibility(Method::PUBLIC);
    append.addParam("bean", this->model.getModule() + "_Model_Bean_" + this->model.getName());
    append.addBody("parent::offsetSet($bean->get" + this->ucfirst(this->getPrimaryKey().getField())+"(), $bean);");

    docblock.setShortDescription("Append " + this->model.getModule() + "_Model_Bean_" + this->model.getName());
    docblock.addTag("param", this->model.getModule() + "_Model_Bean_" + this->model.getName() + " $bean");
    append.setDocblock(docblock);

    return append;
}

Method ZfCollection::getIsEmpty()
{
    Method method;
    Docblock docblock;
    method.setName("isEmpty");
    method.setVisibility(Method::PUBLIC);
    //method.addParam("bean", this->model.getModule() + "_Model_Bean_" + this->model.getName());
    method.addBody("return $this->count() == 0;");

    docblock.setShortDescription("Check if this collection is empty");
    docblock.addTag("return", "Boolean");
    method.setDocblock(docblock);

    return method;
}

Method ZfCollection::getRemove()
{
    Method method;
    Docblock docblock;
    method.setName("remove");
    method.setVisibility(Method::PUBLIC);
    method.addParam("index","int");
    method.addBody("$this->offsetUnset($index);");

    docblock.setShortDescription("Remove one bean of Collecition");
    //docblock.addTag("return", "Boolean");
    method.setDocblock(docblock);

    return method;
}

ColumnBean ZfCollection::getPrimaryKey()
{
    TableCatalog t;
    this->columns = t.getColumnsByTable(this->model.getTable());
    ColumnBean column;
    foreach (column, this->columns) {
        if(column.isPkAutoIncrement())
            return column;
    }
}
