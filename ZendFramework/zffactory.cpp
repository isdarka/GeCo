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
    if(!this->model.getExtend().isEmpty())
    {
        extra = this->getByExntendName(this->model.getExtend());
        this->code.setExtends(extra.getName() + "Factory");
    }else
        this->code.setExtends("AbstractFactory");







    Method methodCreateFromArray;
    Docblock docblockTCreateFromArray;
    docblockTCreateFromArray.setShortDescription("Create " + this->model.getName() + " from array");
    docblockTCreateFromArray.addTag("return",this->model.getName());
    methodCreateFromArray.setName("createFromArray");
    methodCreateFromArray.setVisibility(Method::PUBLIC);
    methodCreateFromArray.isStatic(true);
    methodCreateFromArray.addParam("fields","array");
    methodCreateFromArray.addBody("$" + this->lcFirst(this->model.getName()) + " = new " + this->model.getName() + "();");
    methodCreateFromArray.addBody("self::populate($" + this->lcFirst(this->model.getName()) + ",$fields);");
    methodCreateFromArray.addBody("return $" + this->lcFirst(this->model.getName()) + ";");
    methodCreateFromArray.setDocblock(docblockTCreateFromArray);
    this->code.addMethod(methodCreateFromArray);


    Method methodPopulate;
    Docblock docblockPopulate;
    docblockPopulate.setShortDescription("Populate " + this->model.getName() + "");
//    docblockPopulate.addTag("return",this->model.getName());
    methodPopulate.setName("populate");
    methodPopulate.setVisibility(Method::PUBLIC);
    methodPopulate.isStatic(true);
    methodPopulate.addParam(this->lcFirst(this->model.getName()));
    methodPopulate.addParam("fields");


    methodPopulate.addBody("if( !($" + this->lcFirst(this->model.getName()) +" instanceof " + this->model.getName() +") )");
    methodPopulate.addBody("\tthrow new ActionException('$" + this->lcFirst(this->model.getName()) +" must be instance of " + this->model.getName() +"');");
    methodPopulate.addBody("");
    TableCatalog t;
    this->columns = t.getColumnsByTable(this->model.getTable());
    ColumnBean column;
    foreach (column, this->columns) {
        methodPopulate.addBody("if( isset($fields[" + this->model.getName() +"::" + column.getField().toUpper() +"]) ){");
            methodPopulate.addBody("\t$" + this->lcFirst(this->model.getName()) +"->set" + this->ucfirst(column.getField()) +"($fields[" + this->model.getName() +"::" + column.getField().toUpper() +"]);");
        methodPopulate.addBody("}");
        methodPopulate.addBody("");
    }

    methodPopulate.setDocblock(docblockPopulate);
    this->code.addMethod(methodPopulate);

//        public static function populate($action, $fields)
//        {
//            if( !($action instanceof Action) )
//                throw new ActionException('$action must be instance of Action');

//            if( isset($fields[Action::ID_ACTION]) ){
//                $action->setIdAction($fields[Action::ID_ACTION]);
//            }

//            if( isset($fields[Action::ID_CONTROLLER]) ){
//                $action->setIdController($fields[Action::ID_CONTROLLER]);
//            }

//            if( isset($fields[Action::NAME]) ){
//                $action->setName($fields[Action::NAME]);
//            }

//        }

    Docblock docblock;
    docblock.setShortDescription(this->model.getName() + "Bean");
    docblock.setLongDescription("GeCo");
    docblock.addTag("autor", "isdarka");
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
            this->code.addUse(extra.getModule() + "\\Model\\Factory\\" + extra.getName() + "\\Factory");


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
