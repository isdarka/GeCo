#include "zfcatalog.h"


ZfCatalog::ZfCatalog(GeCoBean model, QVector<GeCoBean> models)
{
    this->model = model;
    this->models = models;

}



QString ZfCatalog::ucfirst(QString str)
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

QString ZfCatalog::lcFirst(QString str)
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

void ZfCatalog::generate()
{

    bool isExtended = false;
    GeCoBean model;
    foreach (model, this->models) {
        if(this->model.getName() == model.getExtend())
            isExtended = true;
    }
    this->code.addUse("Model\\Bean\\AbstractBean");
    this->code.addUse(this->model.getModule() + "\\Model\\Metadata\\" + this->model.getName() + "Metadata");
    this->code.addUse(this->model.getModule() + "\\Model\\Exception\\" + this->model.getName() + "Exception");
    this->code.addUse("Zend\\Db\\Sql\\Where");
    GeCoBean extra;
    this->code.setClassName(this->model.getName() + "Catalog");
    if(!this->model.getExtend().isEmpty())
    {
        extra = this->getByExntendName(this->model.getExtend());
        this->code.setExtends(extra.getName() + "Catalog");
        this->code.addUse(extra.getModule() + "\\Model\\Catalog\\" + extra.getName() + "Catalog");
    }else{
        this->code.setExtends("AbstractCatalog");
        this->code.addUse("Model\\Catalog\\AbstractCatalog");
    }


    ColumnBean primaryKey;
    ColumnBean column;
    foreach (column, this->columns) {
        if(column.isPkAutoIncrement())
            primaryKey = column;
    }

    Method methodGetMetadatay;
    Docblock docblockGetMetadatay;
    docblockGetMetadatay.setShortDescription("Get " + this->model.getName() + "Metadata");
    docblockGetMetadatay.addTag("return",this->model.getName() + "Metadata");
    methodGetMetadatay.setName("getMetadata");
    methodGetMetadatay.setVisibility(Method::PUBLIC);
    methodGetMetadatay.isStatic(false);
    methodGetMetadatay.addBody("return new " + this->model.getName() + "Metadata();");
    methodGetMetadatay.setDocblock(docblockGetMetadatay);
    this->code.addMethod(methodGetMetadatay);

    if(!this->model.getExtend().isEmpty() || isExtended)
    {
        Method methodCreate;
        Docblock docblockCreate;
        docblockCreate.setShortDescription("Create " + this->model.getName() + "");
        methodCreate.setName("create");
        methodCreate.addParam("bean", "AbstractBean");
        methodCreate.setVisibility(Method::PROTECTED);
        methodCreate.isStatic(false);
        methodCreate.addBody("try {");
        if(!this->model.getExtend().isEmpty())
            methodCreate.addBody("\tparent::create($bean);");
        methodCreate.addBody("\t$this->insert = $this->sql->insert(self::getMetadata()->getTableName());");
        methodCreate.addBody("\t$data = self::getMetadata()->toCreateArray($bean);");
        methodCreate.addBody("\t$data = array_filter($data, array($this, 'isNotNull'));");
        methodCreate.addBody("\t$this->insert->values($data);");
        methodCreate.addBody("\t$this->execute($this->insert);");
        methodCreate.addBody("\t$this->getMetadata()->getFactory()->populate($bean, array(");
        methodCreate.addBody("\t\tself::getMetadata()->getPrimaryKey() => $this->getLastInsertId(),");
        methodCreate.addBody("\t));");
        methodCreate.addBody("} catch (\\Zend\\Db\\Exception\\ExceptionInterface $e) {");
        methodCreate.addBody("\tthrow $e;");
        methodCreate.addBody("}");
        methodCreate.addBody("} catch (" + this->model.getName() + "Exception $e) {");
        methodCreate.addBody("\tthrow $e;");
        methodCreate.addBody("}");
        methodCreate.setDocblock(docblockCreate);
        this->code.addMethod(methodCreate);


        Method methodUpdate;
        Docblock docblockUpdate;
        docblockUpdate.setShortDescription("Update " + this->model.getName() + "");
        methodUpdate.setName("create");
        methodUpdate.addParam("bean", "AbstractBean");
        methodUpdate.setVisibility(Method::PROTECTED);
        methodUpdate.isStatic(false);
        methodUpdate.addBody("try {");
        if(!this->model.getExtend().isEmpty())
            methodUpdate.addBody("\tparent::create($bean);");
        methodUpdate.addBody("\t$this->update = $this->sql->update(self::getMetadata()->getTableName());");
        methodUpdate.addBody("\t$data = self::getMetadata()->toUpdateArray($bean);");
        methodUpdate.addBody("\t$data = array_filter($data, array($this, 'isNotNull'));");
        methodUpdate.addBody("\t$this->update->set($data);");
        methodUpdate.addBody("\t$where = new Where();");
        methodUpdate.addBody("\t$where->equalTo(self::getMetadata()->getPrimaryKey(), $bean->get" + this->ucfirst(primaryKey.getField())+"());");
        methodUpdate.addBody("\t$this->update->where($where);");
        methodUpdate.addBody("\t$this->execute($this->update);");


        methodUpdate.addBody("} catch (\\Zend\\Db\\Exception\\ExceptionInterface $e) {");
        methodUpdate.addBody("\tthrow $e;");
        methodUpdate.addBody("}");
        methodUpdate.addBody("} catch (" + this->model.getName() + "Exception $e) {");
        methodUpdate.addBody("\tthrow $e;");
        methodUpdate.addBody("}");
        methodUpdate.setDocblock(docblockUpdate);
        this->code.addMethod(methodUpdate);
    }



    Docblock docblock;
    docblock.setShortDescription(this->model.getName() + "Catalog");
    docblock.setLongDescription("GeCo");
    docblock.addTag("autor", "isdarka");
    docblock.addTag("category", "Model");
    docblock.addTag("package", "Catalog");
    docblock.addTag("copyright", "");
    docblock.addTag("license", "");
    docblock.addTag("created", QDateTime::currentDateTime().toString());
    docblock.addTag("version", "1.0");

    this->code.setDocblock(docblock);

    this->code.setNamespace(this->model.getModule() + "\\Model\\Catalog");
    if(!this->model.getExtend().isEmpty())
        if(this->model.getModule() != extra.getModule())
            this->code.addUse(extra.getModule() + "\\Model\\Catalog\\" + extra.getName() + "Catalog");


}

GeCoBean ZfCatalog::getByExntendName(QString extend)
{
    GeCoBean model;
    foreach (model, this->models) {
        if(model.getName() == extend)
            return model;
    }
}

void ZfCatalog::write( QString str)
{
    QFile file(str);
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream out(&file);
        out << this->code.generate();

        // optional, as QFile destructor will already do it:
        file.close();
}
