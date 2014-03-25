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

void ZfCatalog::generate(QString prefixString)
{
    QStringList prefixes = prefixString.split(",");
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
       // this->code.addUse(extra.getModule() + "\\Model\\Catalog\\" + extra.getName() + "Catalog");
    }else{
        this->code.setExtends("AbstractCatalog");
        this->code.addUse("Model\\Catalog\\AbstractCatalog");
    }

    TableCatalog t;
    this->columns = t.getColumnsByTable(this->model.getTable());
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
        methodCreate.addBody("\t$data = array_map(array($this, 'filterTags'), $data);");

        methodCreate.addBody("\t$this->insert->values($data);");
        methodCreate.addBody("\t$this->execute($this->insert);");
        methodCreate.addBody("\t$this->getMetadata()->getFactory()->populate($bean, array(");
        methodCreate.addBody("\t\tself::getMetadata()->getPrimaryKey() => $this->getLastInsertId(),");
        methodCreate.addBody("\t));");
        methodCreate.addBody("} catch (\\Zend\\Db\\Exception\\ExceptionInterface $e) {");
        methodCreate.addBody("\tthrow $e;");
        methodCreate.addBody("} catch (" + this->model.getName() + "Exception $e) {");
        methodCreate.addBody("\tthrow $e;");
        methodCreate.addBody("}");
        methodCreate.setDocblock(docblockCreate);
        this->code.addMethod(methodCreate);


        Method methodUpdate;
        Docblock docblockUpdate;
        docblockUpdate.setShortDescription("Update " + this->model.getName() + "");
        methodUpdate.setName("update");
        methodUpdate.addParam("bean", "AbstractBean");
        methodUpdate.setVisibility(Method::PROTECTED);
        methodUpdate.isStatic(false);
        methodUpdate.addBody("try {");
        if(!this->model.getExtend().isEmpty())
            methodUpdate.addBody("\tparent::update($bean);");
        methodUpdate.addBody("\t$this->update = $this->sql->update(self::getMetadata()->getTableName());");
        methodUpdate.addBody("\t$data = self::getMetadata()->toUpdateArray($bean);");
        methodUpdate.addBody("\t$data = array_filter($data, array($this, 'isNotNull'));");
        methodUpdate.addBody("\t$data = array_map(array($this, 'filterTags'), $data);");
        methodUpdate.addBody("\t$this->update->set($data);");
        methodUpdate.addBody("\t$where = new Where();");
        methodUpdate.addBody("\t$where->equalTo(self::getMetadata()->getPrimaryKey(), $bean->get" + this->ucfirst(primaryKey.getField())+"());");
        methodUpdate.addBody("\t$this->update->where($where);");
        methodUpdate.addBody("\t$this->execute($this->update);");


        methodUpdate.addBody("} catch (\\Zend\\Db\\Exception\\ExceptionInterface $e) {");
        methodUpdate.addBody("\tthrow $e;");
        methodUpdate.addBody("} catch (" + this->model.getName() + "Exception $e) {");
        methodUpdate.addBody("\tthrow $e;");
        methodUpdate.addBody("}");
        methodUpdate.setDocblock(docblockUpdate);
        this->code.addMethod(methodUpdate);


        Method methodDelete;
        Docblock docblockDelete;
        docblockDelete.setShortDescription("Delete " + this->model.getName() + "");
        methodDelete.setName("delete");
        methodDelete.addParam("bean", "AbstractBean");
        methodDelete.setVisibility(Method::PUBLIC);
        methodDelete.isStatic(false);
        methodDelete.addBody("try {");
        if(!this->model.getExtend().isEmpty())
            methodDelete.addBody("\tparent::delete($bean);");
        methodDelete.addBody("\t$this->delete = $this->sql->delete($this->getMetadata()->getTableName());");
        methodDelete.addBody("\t$where = new Where();");
        methodDelete.addBody("\t$where->equalTo(self::getMetadata()->getPrimaryKey(), $bean->get" + this->ucfirst(primaryKey.getField())+"());");
        methodDelete.addBody("\t$this->delete->where($where);");
        methodDelete.addBody("\t$this->execute($this->delete);");
        methodDelete.addBody("} catch (\\Zend\\Db\\Exception\\ExceptionInterface $e) {");
        methodDelete.addBody("\tthrow $e;");
        methodDelete.addBody("} catch (" + this->model.getName() + "Exception $e) {");
        methodDelete.addBody("\tthrow $e;");
        methodDelete.addBody("}");
        methodDelete.setDocblock(docblockDelete);
        this->code.addMethod(methodDelete);
    }

QRegExp exp("([A-Z])");
    //RELATIONS
    QString relation;
    bool hasRelations = false;
    foreach (relation, this->model.getRelations()) {
        if(!relation.isEmpty())
        {
            hasRelations = true;
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

            QSqlQuery query;
            QStringList fields;
            query.exec("DESC " + tmp);


            while(query.next()) {
                fields.append(query.value(0).toString());
            }

            QString field;

            Method linkTo;
            QString fieldRelation;
            Docblock docblockLinkTo;
            docblockLinkTo.setShortDescription("Link to " + this->ucfirst(relation));
            linkTo.setName("linkTo"+ this->ucfirst(relation));
            linkTo.setVisibility(Method::PUBLIC);
            foreach (field, fields) {
                if(field != primaryKey.getField())
                    fieldRelation = field;

                linkTo.addParam(this->lcFirst(this->ucfirst(field)));
            }

            linkTo.isStatic(false);
            linkTo.addBody("try {");
            linkTo.addBody("\t$this->insert = $this->sql->insert('" + tmp + "');");
            linkTo.addBody("\t$this->insert->values(array(");

            foreach (field, fields) {
                linkTo.addBody("\t\t'" + field + "' => $" + this->lcFirst(this->ucfirst(field)) + ",");
            }

            linkTo.addBody("\t));");
            linkTo.addBody("\t$this->execute($this->insert);");
            linkTo.addBody("}catch (\\Zend\\Db\\Exception\\ExceptionInterface $e) {");
            linkTo.addBody("\tthrow $e;");
            linkTo.addBody("} catch (\\Exception $e) {");
            linkTo.addBody("\tthrow $e;");
            linkTo.addBody("}");
            linkTo.setDocblock(docblockLinkTo);
            this->code.addMethod(linkTo);

            Method unlinkFrom;
            Docblock docblockUnlinkFrom;
            docblockUnlinkFrom.setShortDescription("Unlink from " + this->model.getName());
            unlinkFrom.setName("unlinkFrom"+ this->ucfirst(relation));
            unlinkFrom.setVisibility(Method::PUBLIC);
            foreach (field, fields) {
                unlinkFrom.addParam(this->lcFirst(this->ucfirst(field)));
            }

            unlinkFrom.isStatic(false);
            unlinkFrom.addBody("try {");
            unlinkFrom.addBody("\t$this->delete = $this->sql->delete('" + tmp + "');");
            unlinkFrom.addBody("\t$where = new Where();");

            foreach (field, fields) {
                unlinkFrom.addBody("\t$where->equalTo('" + field + "', $" + this->lcFirst(this->ucfirst(field)) + ");");
            }

            unlinkFrom.addBody("\t$this->delete->where($where);");
            unlinkFrom.addBody("\t$this->execute($this->delete);");
            unlinkFrom.addBody("}catch (\\Zend\\Db\\Exception\\ExceptionInterface $e) {");
            unlinkFrom.addBody("\tthrow $e;");
            unlinkFrom.addBody("} catch (\\Exception $e) {");
            unlinkFrom.addBody("\tthrow $e;");
            unlinkFrom.addBody("}");
            unlinkFrom.setDocblock(docblockUnlinkFrom);
            this->code.addMethod(unlinkFrom);


            Method unlinkAll;
            Docblock docblockUnlinkAll;
            docblockUnlinkAll.setShortDescription("Unlink all " + this->model.getName());
            unlinkAll.setName("unlinkAll"+ this->ucfirst(relation));
            unlinkAll.setVisibility(Method::PUBLIC);

                unlinkAll.addParam("id" + this->model.getName());


            unlinkAll.isStatic(false);
            unlinkAll.addBody("try {");
            unlinkAll.addBody("\t$this->delete = $this->sql->delete('" + tmp + "');");
            unlinkAll.addBody("\t$where = new Where();");


                unlinkAll.addBody("\t$where->equalTo('" + primaryKey.getField() + "', $id" + this->model.getName() + ");");


            unlinkAll.addBody("\t$this->delete->where($where);");
            unlinkAll.addBody("\t$this->execute($this->delete);");
            unlinkAll.addBody("}catch (\\Zend\\Db\\Exception\\ExceptionInterface $e) {");
            unlinkAll.addBody("\tthrow $e;");
            unlinkAll.addBody("} catch (\\Exception $e) {");
            unlinkAll.addBody("\tthrow $e;");
            unlinkAll.addBody("}");
            unlinkAll.setDocblock(docblockUnlinkAll);
            this->code.addMethod(unlinkAll);
        }
    }

   // if(hasRelations)
     //   this->code.addUse("Zend\\Db\\Sql\\Where");
    // /RELATIONS

    Docblock docblock;
    docblock.setShortDescription(this->model.getName() + "Catalog");
    docblock.setLongDescription("GeCo");
    docblock.addTag("author", "isdarka");
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
