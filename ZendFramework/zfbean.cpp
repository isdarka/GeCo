#include "zfbean.h"

ZfBean::ZfBean(GeCoBean model, QVector<GeCoBean> models)
{
    this->model = model;
    this->models = models;

}



QString ZfBean::ucfirst(QString str)
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

QString ZfBean::lcFirst(QString str)
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

void ZfBean::generate()
{

    GeCoBean extra;
    bool hasStatus = false;
    this->code.setClassName(this->model.getName());
    if(!this->model.getExtend().isEmpty())
    {
        extra = this->getByExntendName(this->model.getExtend());
     //   qDebug() << extra.getModule();
        this->code.setExtends(extra.getName());
    }else
        this->code.setExtends("AbstractBean");

    TableCatalog t;
    this->columns = t.getColumnsByTable(this->model.getTable());
    ColumnBean column;

    //Constants
    Propertie properties;
    properties.setName("TABLENAME");
    //properties.setVisibility(Propertie::PRIVATE);
    properties.isConst(true);
    properties.setDefaultValue(this->model.getTable());
    this->code.addPropertie(properties);
    foreach (column, this->columns) {
        if(column.getField() == "status")
            hasStatus = true;
//        qDebug() << column.getField();
//        qDebug() << column.getDefault();
        Propertie properties;        
        properties.setName(column.getField().toUpper());
       //properties.setVisibility(Propertie::PRIVATE);
        properties.isConst(true);
        properties.setDefaultValue(column.getField());
        this->code.addPropertie(properties);
    }

    if(hasStatus)
    {
        Propertie enable;
        enable.setName("ENABLE");
        enable.isConst(true);
        enable.setDefaultValue("1");
        this->code.addPropertie(enable);

        Propertie disable;
        disable.setName("DISABLE");
        disable.isConst(true);
        disable.setDefaultValue("2");
        this->code.addPropertie(disable);
    }
    //PRIVATES
    ColumnBean primaryKey;
    foreach (column, this->columns) {
//        qDebug() << column.getField();
//        qDebug() << column.getDefault();
        if(column.isPkAutoIncrement())
            primaryKey = column;
        Propertie properties;
        properties.setName(this->lcFirst(this->ucfirst(column.getField())));
        properties.setVisibility(Propertie::PRIVATE);
        properties.isConst(false);
        properties.setDefaultValue(column.getDefault());

        this->code.addPropertie(properties);
    }



    //GetIndex

    Method method;
    Docblock docblockIndex;
    docblockIndex.setShortDescription("Get the " + this->lcFirst(this->ucfirst(primaryKey.getField())) + " property");
    docblockIndex.addTag("return","int $" + this->lcFirst(this->ucfirst(primaryKey.getField())));

    method.setName("getIndex");
    method.setVisibility(Method::PUBLIC);
    method.addBody("return $this->" + this->lcFirst(this->ucfirst(primaryKey.getField())) + ";");
    method.setDocblock(docblockIndex);
    this->code.addMethod(method);




    foreach (column, this->columns) {
        Method method;
        Docblock docblock;
        docblock.setShortDescription("Set the " + this->lcFirst(this->ucfirst(column.getField())) + " property");
        if(column.isDate())
            docblock.addTag("param","Zend_Date $" + this->lcFirst(this->ucfirst(column.getField())));
        else if(column.isNumber())
            docblock.addTag("param","int $" + this->lcFirst(this->ucfirst(column.getField())));
        else if(column.isTime())
            docblock.addTag("param","Zend_Date $" + this->lcFirst(this->ucfirst(column.getField())));
        else if(column.isLongText())
            docblock.addTag("param","string $" + this->lcFirst(this->ucfirst(column.getField())));
        else if(column.isText())
            docblock.addTag("param","string $" + this->lcFirst(this->ucfirst(column.getField())));
        else
            docblock.addTag("param","unknow $" + this->lcFirst(this->ucfirst(column.getField())));

        method.setDocblock(docblock);
        method.setName("set" + (this->ucfirst(column.getField())));
        method.setVisibility(Method::PUBLIC);
        if(column.isDate())
            method.addParam(this->lcFirst(this->ucfirst(column.getField())),"Zend_Date");
        else if(column.isTime())
            method.addParam(this->lcFirst(this->ucfirst(column.getField())),"Zend_Date");
        else
            method.addParam(this->lcFirst(this->ucfirst(column.getField())));

        method.addBody("$this->" + this->lcFirst(this->ucfirst(column.getField())) + " = $" + this->lcFirst(this->ucfirst(column.getField())) + ";");
        method.addBody("return $this;");
        this->code.addMethod(method);
    }

    foreach (column, this->columns) {
        Method method;
        Docblock docblock;
        docblock.setShortDescription("Get the " + this->lcFirst(this->ucfirst(column.getField())) + " property");
        if(column.isDate())
            docblock.addTag("return","Zend_Date $" + this->lcFirst(this->ucfirst(column.getField())));
        else if(column.isNumber())
            docblock.addTag("return","int $" + this->lcFirst(this->ucfirst(column.getField())));
        else if(column.isTime())
            docblock.addTag("return","Zend_Date $" + this->lcFirst(this->ucfirst(column.getField())));
        else if(column.isLongText())
            docblock.addTag("return","string $" + this->lcFirst(this->ucfirst(column.getField())));
        else if(column.isText())
            docblock.addTag("return","string $" + this->lcFirst(this->ucfirst(column.getField())));
        else
            docblock.addTag("return","unknow $" + this->lcFirst(this->ucfirst(column.getField())));
       // docblock.addTag("return",column.getType());
        method.setName("get" + this->ucfirst(column.getField()));
        method.setVisibility(Method::PUBLIC);
        method.addBody("return $this->" + this->lcFirst(this->ucfirst(column.getField())) + ";");
        method.setDocblock(docblock);
        this->code.addMethod(method);
    }

        //toArray
    Method methodToArray;
    Docblock docblockToArray;
    docblockToArray.setShortDescription("Get the Array");
    docblockToArray.addTag("return","array");
    methodToArray.setName("toArray");
    methodToArray.setVisibility(Method::PUBLIC);
    methodToArray.addBody("$array = array(");
    foreach (column, this->columns) {
        methodToArray.addBody("\tself::" + column.getField().toUpper() + " => " + "$this->get" + (this->ucfirst(column.getField())) + "(),");
    }
    methodToArray.addBody(");");
    if(this->model.getExtend().isEmpty())
        methodToArray.addBody("return $array;");
    else
        methodToArray.addBody("return array_merge(parent::toArray(), $array);");

    methodToArray.setDocblock(docblockToArray);
    this->code.addMethod(methodToArray);

       Docblock docblock;
       docblock.setShortDescription(this->model.getName() + "Bean");
       docblock.setLongDescription("GeCo");
       docblock.addTag("autor", "isdarka");
       docblock.addTag("category", "Model");
       docblock.addTag("package", "Bean");
       docblock.addTag("copyright", "");
       docblock.addTag("license", "");
       docblock.addTag("created", QDateTime::currentDateTime().toString());
       docblock.addTag("version", "1.0");

       this->code.setDocblock(docblock);

       this->code.setNamespace(this->model.getModule() + "\\Model\\Bean");
       if(this->model.getExtend().isEmpty())
            this->code.addUse("Model\\Bean\\AbstractBean");
       else{
           if(this->model.getModule() != extra.getModule())
               this->code.addUse(extra.getModule() + "\\Model\\Bean\\" + extra.getName());

       }

       if(hasStatus)
       {

           Method statuses;
           Docblock docblockStatuses;
           docblockStatuses.setShortDescription("Statuses");

           statuses.setName("$statuses");
           statuses.setVisibility(Method::PUBLIC);
           statuses.isStatic(true);
           statuses.isFunction(false);
           statuses.addParam("self::ENABLE => \"Enable\"");
           statuses.addParam("self::DISABLE => \"Disable\"");
           statuses.addBody("");
           statuses.setDocblock(docblockStatuses);
           this->code.addMethod(statuses);

           Method enable;
           Docblock docblockenable;
           docblockenable.setShortDescription(this->lcFirst(this->ucfirst(primaryKey.getField())) + " is enable");
           docblockenable.addTag("return","boolean");

           enable.setName("isEnabled");
           enable.setVisibility(Method::PUBLIC);
           enable.addBody("return self::ENABLE == $this->getStatus();");
           enable.setDocblock(docblockenable);
           this->code.addMethod(enable);

           Method disable;
           Docblock docblockDisable;
           docblockDisable.setShortDescription(this->lcFirst(this->ucfirst(primaryKey.getField())) + " is disable");
           docblockDisable.addTag("return","boolean");

           disable.setName("isDisabled");
           disable.setVisibility(Method::PUBLIC);
           disable.addBody("return self::DISABLE == $this->getStatus();");
           disable.setDocblock(docblockDisable);
           this->code.addMethod(disable);

           Method statusName;
           Docblock docblockStatusName;
           docblockStatusName.setShortDescription("Get Status Name");
           docblockStatusName.addTag("return","string");

           statusName.setName("getStatusString");
           statusName.setVisibility(Method::PUBLIC);
           statusName.addBody("return self::$statuses[$this->getStatus()];");
           statusName.setDocblock(docblockStatusName);
           this->code.addMethod(statusName);
       }
}

GeCoBean ZfBean::getByExntendName(QString extend)
{
    GeCoBean model;
    foreach (model, this->models) {
        if(model.getName() == extend)
            return model;
    }
}

void ZfBean::write( QString str)
{   
    QFile file(str);
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream out(&file);
        out << this->code.generate();

        // optional, as QFile destructor will already do it:
        file.close();
}

