#include "zfcollection.h"



ZfCollection::ZfCollection(GeCoBean model, QVector<GeCoBean> models)
{
    this->model = model;
    this->models = models;

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

QString ZfCollection::lcFirst(QString str)
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

void ZfCollection::generate()
{

    GeCoBean extra;
    this->code.setClassName(this->model.getName() + "Collection");
    if(!this->model.getExtend().isEmpty())
    {
        extra = this->getByExntendName(this->model.getExtend());
        this->code.setExtends(extra.getName() + "Collection");
    }else
        this->code.setExtends("AbstractCollection");


    TableCatalog t;
    this->columns = t.getColumnsByTable(this->model.getTable());
    ColumnBean column;

    foreach (column, this->columns) {
        if(column.isForeingKey()){
            Method method;
            Docblock docblock;
            docblock.setShortDescription("Get Ids");
            docblock.addTag("return","array");

            method.setName("get" + this->ucfirst(column.getField()).replace("Id", "") + "Ids");
            method.setVisibility(Method::PUBLIC);
            method.addBody("return $this->map(function(" + this->model.getName() + " $" + this->lcFirst(this->model.getName()) + "){");
            method.addBody("\treturn array($" + this->lcFirst(this->model.getName()) + "->get" + this->ucfirst(column.getField()) + "() => $action->get" + this->ucfirst(column.getField()) + "());");
            method.addBody("});");
            method.setDocblock(docblock);
            this->code.addMethod(method);


            Method methodGetBy;
            Docblock docblockGetBy;
            docblockGetBy.setShortDescription("Get Ids");
            docblockGetBy.addTag("return", this->model.getName() + "Collection");

            methodGetBy.setName("getBy" + this->ucfirst(column.getField()));
            methodGetBy.addParam(this->lcFirst(this->ucfirst(column.getField())));
            methodGetBy.setVisibility(Method::PUBLIC);
            methodGetBy.addBody("$" + this->lcFirst(this->model.getName()) + "Collection = new " + this->model.getName() + "Collection();");
            methodGetBy.addBody("$this->each(function(" + this->model.getName() + " $" + this->lcFirst(this->model.getName()) + ") use ($" + this->lcFirst(this->ucfirst(column.getField())) + ", $" + this->lcFirst(this->model.getName()) + "Collection){");
            methodGetBy.addBody("\tif($" + this->lcFirst(this->model.getName()) + "->get" + this->ucfirst(column.getField()) + "() == $" + this->lcFirst(this->ucfirst(column.getField())) + ")");
            methodGetBy.addBody("\t\t$" + this->lcFirst(this->model.getName()) + "Collection->append($" + this->lcFirst(this->model.getName()) + ");");
            methodGetBy.addBody("});");
            methodGetBy.addBody("return $" + this->lcFirst(this->model.getName()) + "Collection;");
            methodGetBy.setDocblock(docblockGetBy);
            this->code.addMethod(methodGetBy);
        }
    }

//    public function getAlgoIds()
//        {
//            return $this->map(function(Action $action){
//                return array($action->getIdController() => $action->getIdController());
//            });
//        }

//        public function getByIdController($idController)
//        {
//            $actionCollection = new ActionCollection();
//            $this->each(function(Action $action) use ($idController, $actionCollection){
//                if($action->getIdController() == $idController)
//                    $actionCollection->append($action);
//            });

//            return $actionCollection;
//        }
    Docblock docblock;
    docblock.setShortDescription(this->model.getName() + "Bean");
    docblock.setLongDescription("GeCo");
    docblock.addTag("autor", "isdarka");
    docblock.addTag("category", "Model");
    docblock.addTag("package", "Collection");
    docblock.addTag("copyright", "");
    docblock.addTag("license", "");
    docblock.addTag("created", QDateTime::currentDateTime().toString());
    docblock.addTag("version", "1.0");

    this->code.setDocblock(docblock);

    this->code.setNamespace(this->model.getModule() + "\\Model\\Collection");
    if(!this->model.getExtend().isEmpty()){
        if(this->model.getModule() != extra.getModule())
            this->code.addUse(extra.getModule() + "\\Model\\Collection\\" + extra.getName() + "Collection");
    }else
        this->code.addUse("Model\\Collection\\AbstractCollection");



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

