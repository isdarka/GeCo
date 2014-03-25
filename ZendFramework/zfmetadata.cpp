#include "zfmetadata.h"


ZfMetadata::ZfMetadata(GeCoBean model, QVector<GeCoBean> models)
{
    this->model = model;
    this->models = models;

}



QString ZfMetadata::ucfirst(QString str)
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

QString ZfMetadata::lcFirst(QString str)
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

void ZfMetadata::generate()
{

    GeCoBean extra;
    this->code.setClassName(this->model.getName() + "Metadata");
//    if(!this->model.getExtend().isEmpty())
//    {
//        extra = this->getByExntendName(this->model.getExtend());
//        this->code.setExtends(extra.getName() + "Metadata");
//    }else
        this->code.setExtends("AbstractMetadata");


    this->code.addUse("Model\\Bean\\AbstractBean");
    TableCatalog t;
    this->columns = t.getColumnsByTable(this->model.getTable());
    ColumnBean column;
    ColumnBean primaryKey;

    Method methodToUpdateArray;
    Docblock docblockToUpdateArray;
    docblockToUpdateArray.setShortDescription("toUpdateArray");
    methodToUpdateArray.setName("toUpdateArray");
    methodToUpdateArray.setVisibility(Method::PUBLIC);
    methodToUpdateArray.isStatic(true);
    methodToUpdateArray.addParam("bean", "AbstractBean");
    methodToUpdateArray.addBody("return $bean->toArrayFor(");
    methodToUpdateArray.addBody("\tarray(");
    foreach (column, this->columns) {
        if(column.isPkAutoIncrement())
            primaryKey = column;
        else
            methodToUpdateArray.addBody("\t\t" + this->model.getName() +"::" + column.getField().toUpper() + ",");
    }
    methodToUpdateArray.addBody("\t)");
    methodToUpdateArray.addBody(");");
    methodToUpdateArray.setDocblock(docblockToUpdateArray);
    this->code.addMethod(methodToUpdateArray);



    Method methodToCreateArray;
    Docblock docblockToCreateArray;
    docblockToCreateArray.setShortDescription("toCreateArray");
    methodToCreateArray.setName("toCreateArray");
    methodToCreateArray.setVisibility(Method::PUBLIC);
    methodToCreateArray.isStatic(true);
    methodToCreateArray.addParam("bean", "AbstractBean");
    methodToCreateArray.addBody("return $bean->toArrayFor(");
    methodToCreateArray.addBody("\tarray(");
    foreach (column, this->columns) {
            methodToCreateArray.addBody("\t\t" + this->model.getName() +"::" + column.getField().toUpper() + ",");
    }
    methodToCreateArray.addBody("\t)");
    methodToCreateArray.addBody(");");
    methodToCreateArray.setDocblock(docblockToCreateArray);
    this->code.addMethod(methodToCreateArray);


    Method methodGetFields;
    Docblock docblockGetFields;
    docblockGetFields.setShortDescription("Get fields");
    methodGetFields.setName("getFields");
    methodGetFields.setVisibility(Method::PUBLIC);
    methodGetFields.isStatic(true);

    methodGetFields.addBody("return array(");
    foreach (column, this->columns) {
            methodGetFields.addBody("\t" + this->model.getName() +"::" + column.getField().toUpper() + ",");
    }
    methodGetFields.addBody(");");
    methodGetFields.setDocblock(docblockGetFields);
    this->code.addMethod(methodGetFields);

    Method methodEntityName;
    Docblock docblockEntityName;
    docblockEntityName.setShortDescription("Get Entity Name");
    docblockEntityName.addTag("return", "string");
    methodEntityName.setName("getEntityName");
    methodEntityName.setVisibility(Method::PUBLIC);
    methodEntityName.isStatic(true);
    methodEntityName.addBody("return \"" + this->model.getName() +"\";");
    methodEntityName.setDocblock(docblockEntityName);
    this->code.addMethod(methodEntityName);


    Method methodTableName;
    Docblock docblockTableName;
    docblockTableName.setShortDescription("Get TableName");
    docblockTableName.addTag("return", "string");
    methodTableName.setName("getTableName");
    methodTableName.setVisibility(Method::PUBLIC);
    methodTableName.isStatic(true);
    methodTableName.addBody("return \"" + this->model.getTable() +"\";");
    methodTableName.setDocblock(docblockTableName);
    this->code.addMethod(methodTableName);


    Method methodPrimaryKey;
    Docblock docblockPrimaryKey;
    docblockPrimaryKey.setShortDescription("Get PrimaryKey");
    docblockPrimaryKey.addTag("return", "string");
    methodPrimaryKey.setName("getPrimaryKey");
    methodPrimaryKey.setVisibility(Method::PUBLIC);
    methodPrimaryKey.isStatic(true);
    methodPrimaryKey.addBody("return \"" + primaryKey.getField() +"\";");
    methodPrimaryKey.setDocblock(docblockPrimaryKey);
    this->code.addMethod(methodPrimaryKey);



    Method methodGetFactory;
    Docblock docblockGetFactory;
    docblockGetFactory.setShortDescription("Get Factory");
    docblockGetFactory.addTag("return", this->model.getName() + "Factory");
    methodGetFactory.setName("getFactory");
    methodGetFactory.setVisibility(Method::PUBLIC);
    methodGetFactory.isStatic(true);
    methodGetFactory.addBody("static $factory = null;");
    methodGetFactory.addBody("if( null == $factory ){");
    methodGetFactory.addBody("\t$factory = new " + this->model.getName() +"Factory();");
    methodGetFactory.addBody("}");
    methodGetFactory.addBody("return $factory;");
    methodGetFactory.setDocblock(docblockGetFactory);
    this->code.addMethod(methodGetFactory);

    Method methodNewCollection;
    Docblock docblockNewCollection;
    docblockNewCollection.setShortDescription("Get Collection");
    docblockNewCollection.addTag("return", this->model.getName() + "Collection");
    methodNewCollection.setName("newCollection");
    methodNewCollection.setVisibility(Method::PUBLIC);
    methodNewCollection.isStatic(true);
    methodNewCollection.addBody("return new " + this->model.getName() +"Collection();");
    methodNewCollection.setDocblock(docblockNewCollection);
    this->code.addMethod(methodNewCollection);


    Method methodNewBean;
    Docblock docblockNewBean;
    docblockNewBean.setShortDescription("Get Bean");
    docblockNewBean.addTag("return", this->model.getName() + "");
    methodNewBean.setName("newBean");
    methodNewBean.setVisibility(Method::PUBLIC);
    methodNewBean.isStatic(true);
    methodNewBean.addBody("return new " + this->model.getName() +"();");
    methodNewBean.setDocblock(docblockNewBean);
    this->code.addMethod(methodNewBean);

    Docblock docblock;
    docblock.setShortDescription(this->model.getName() + "Bean");
    docblock.setLongDescription("GeCo");
    docblock.addTag("author", "isdarka");
    docblock.addTag("category", "Model");
    docblock.addTag("package", "Metadata");
    docblock.addTag("copyright", "");
    docblock.addTag("license", "");
    docblock.addTag("created", QDateTime::currentDateTime().toString());
    docblock.addTag("version", "1.0");

    this->code.setDocblock(docblock);

    this->code.setNamespace(this->model.getModule() + "\\Model\\Metadata");
    this->code.addUse("Model\\Metadata\\AbstractMetadata");
    if(this->model.isDefaultModule())
    {
        this->code.addUse("Application\\Model\\Bean\\" + this->model.getName());
        this->code.addUse("Application\\Model\\Factory\\" + this->model.getName() + "Factory");
        this->code.addUse("Application\\Model\\Collection\\" + this->model.getName() + "Collection");
    }else{
        this->code.addUse(this->model.getModule() + "\\Model\\Bean\\" + this->model.getName());
        this->code.addUse(this->model.getModule() + "\\Model\\Factory\\" + this->model.getName() + "Factory");
        this->code.addUse(this->model.getModule() + "\\Model\\Collection\\" + this->model.getName() + "Collection");
    }



}

GeCoBean ZfMetadata::getByExntendName(QString extend)
{
    GeCoBean model;
    foreach (model, this->models) {
        if(model.getName() == extend)
            return model;
    }
}

void ZfMetadata::write( QString str)
{
    QFile file(str);
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream out(&file);
        out << this->code.generate();

        // optional, as QFile destructor will already do it:
        file.close();
}
