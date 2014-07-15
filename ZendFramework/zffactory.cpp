#include "zffactory.h"


ZfFactory::ZfFactory(GeCoBean model, QVector<GeCoBean> models)
{
    this->model = model;
    this->models = models;

}



QString ZfFactory::ucfirst(QString str)
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

QString ZfFactory::lcFirst(QString str)
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

void ZfFactory::generate()
{

    GeCoBean extra;
    this->code.setClassName(this->model.getName() + "Factory");

    this->code.addUse(this->model.getModule() + "\\Model\\Bean\\" + this->model.getName());
    if(!this->model.getExtend().isEmpty())
    {
        extra = this->getByExntendName(this->model.getExtend());
        this->code.setExtends(extra.getName() + "Factory");
        //this->code.addUse(extra.getModule() + "\\Model\\Factory\\" + extra.getName() + "Factory");
    }else{
        this->code.setExtends("AbstractFactory");
        this->code.addUse("Model\\Factory\\AbstractFactory");
    }

this->code.addUse("Zend\\Db\\Sql\\Expression");

    Method methodCreateFromArray;
    Docblock docblockTCreateFromArray;
    docblockTCreateFromArray.setShortDescription("Create " + this->model.getName() + " from array");
    docblockTCreateFromArray.addTag("return",this->model.getName());
    methodCreateFromArray.setName("createFromArray");
    methodCreateFromArray.setVisibility(Method::PUBLIC);
    methodCreateFromArray.isStatic(true);
    methodCreateFromArray.addParam("fields");
    methodCreateFromArray.addBody("$" + this->lcFirst(this->model.getName()) + " = new " + this->model.getName() + "();");
    methodCreateFromArray.addBody("self::populate($" + this->lcFirst(this->model.getName()) + ",$fields);");
    methodCreateFromArray.addBody("return $" + this->lcFirst(this->model.getName()) + ";");
    methodCreateFromArray.setDocblock(docblockTCreateFromArray);
    this->code.addMethod(methodCreateFromArray);


    Method methodPopulate;
    Docblock docblockPopulate;
    docblockPopulate.setShortDescription("Populate " + this->model.getName() + "");
    methodPopulate.setName("populate");
    methodPopulate.setVisibility(Method::PUBLIC);
    methodPopulate.isStatic(true);
    methodPopulate.addParam(this->lcFirst(this->model.getName()));
    methodPopulate.addParam("fields");

    if(!this->model.getExtend().isEmpty())
        methodPopulate.addBody("parent::populate($" + this->lcFirst(this->model.getName()) +", $fields);");
    methodPopulate.addBody("if(!($" + this->lcFirst(this->model.getName()) +" instanceof " + this->model.getName() +"))");
    methodPopulate.addBody("\tthrow new " + this->model.getName() +"Exception('$" + this->lcFirst(this->model.getName()) +" must be instance of " + this->model.getName() +"');");
    this->code.addUse(this->model.getModule() +"\\Model\\Exception\\" + this->model.getName() +"Exception");
    methodPopulate.addBody("");

    methodPopulate.addBody("if($fields instanceof \\stdClass)");
    methodPopulate.addBody("{");
    methodPopulate.addBody("\t$factory = self::getInstance();");
    methodPopulate.addBody("\t$stdClass = clone $fields;");
    methodPopulate.addBody("\t$fields = array();");
    methodPopulate.addBody("\tforeach ($stdClass as $key => $value)");
    methodPopulate.addBody("\t\t$fields[$factory->getUnderscore($key)] = $value;");
    methodPopulate.addBody("}");
    methodPopulate.addBody("");

    TableCatalog t;
    this->columns = t.getColumnsByTable(this->model.getTable());
    ColumnBean column;
    foreach (column, this->columns) {
        QString condition = "if(isset($fields[" + this->model.getName() +"::" + column.getField().toUpper() +"])";
        if(column.getNull())
            condition.append(" && !empty($fields[" + this->model.getName() +"::" + column.getField().toUpper() +"])");
        condition.append("){");
        methodPopulate.addBody(condition);
            methodPopulate.addBody("\t$" + this->lcFirst(this->model.getName()) +"->set" + this->ucfirst(column.getField()) +"($fields[" + this->model.getName() +"::" + column.getField().toUpper() +"]);");
        if(column.getNull())
        {
            methodPopulate.addBody("}else{");
            methodPopulate.addBody("\t$" + this->lcFirst(this->model.getName()) +"->set" + this->ucfirst(column.getField()) +"(new Expression(\"null\"));");

        }
        methodPopulate.addBody("}");
        methodPopulate.addBody("");
    }

    methodPopulate.setDocblock(docblockPopulate);
    this->code.addMethod(methodPopulate);

    Docblock docblock;
    docblock.setShortDescription(this->model.getName() + "Bean");
    docblock.setLongDescription("GeCo");
    docblock.addTag("author", "isdarka");
    docblock.addTag("category", "Model");
    docblock.addTag("package", "Factory");
    docblock.addTag("copyright", "");
    docblock.addTag("license", "");
    docblock.addTag("created", QDateTime::currentDateTime().toString());
    docblock.addTag("version", "1.0");

    this->code.setDocblock(docblock);

    this->code.setNamespace(this->model.getModule() + "\\Model\\Factory");
    if(!this->model.getExtend().isEmpty())
        if(this->model.getModule() != extra.getModule())
            this->code.addUse(extra.getModule() + "\\Model\\Factory\\" + extra.getName() + "Factory");


}

GeCoBean ZfFactory::getByExntendName(QString extend)
{
    GeCoBean model;
    foreach (model, this->models) {
        if(model.getName() == extend)
            return model;
    }
}

void ZfFactory::write( QString str)
{
    QFile file(str);
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream out(&file);
        out << this->code.generate();

        // optional, as QFile destructor will already do it:
        file.close();
}
