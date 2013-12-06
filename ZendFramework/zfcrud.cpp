#include "zfcrud.h"

ZfCRUD::ZfCRUD(GeCoBean model, QVector<GeCoBean> models)
{
    this->model = model;
    this->models = models;

}



QString ZfCRUD::ucfirst(QString str)
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

QString ZfCRUD::lcFirst(QString str)
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

void ZfCRUD::generate()
{

    GeCoBean extra;
    this->code.setClassName(this->model.getName() + "Controller");
    if(!this->model.getExtend().isEmpty())
        extra = this->getByExntendName(this->model.getExtend());

    this->code.setExtends("BaseController");
    this->code.addUse("BaseProject\\Controller\\BaseController");


    TableCatalog t;
    this->columns = t.getColumnsByTable(this->model.getTable());
    ColumnBean column;
    bool hasStatus = false;
    foreach (column, this->columns) {
        if(column.getField() == "status")
            hasStatus = true;
    }


    Method methodIndex;
    Docblock docblockIndex;
    docblockIndex.setShortDescription("Index");
    methodIndex.setName("indexAction");
    methodIndex.setVisibility(Method::PUBLIC);
    methodIndex.isStatic(false);
    methodIndex.addBody("$" + this->lcFirst(this->model.getName()) +"Query = new " + this->model.getName() +"Query($this->getAdatper());");
    methodIndex.addBody("$total = $" + this->lcFirst(this->model.getName()) +"Query->count();");
    methodIndex.addBody("$page = $this->params()->fromRoute(\"page\", 1);");
    methodIndex.addBody("$" + this->lcFirst(this->model.getName()) +"s = $" + this->lcFirst(this->model.getName()) +"Query->limit($this->maxPerPage)->offset(($page -1) * $this->maxPerPage)->find();");
    methodIndex.addBody("");
    methodIndex.addBody("//Views");
    methodIndex.addBody("$this->view->" + this->lcFirst(this->model.getName()) +"s = $" + this->lcFirst(this->model.getName()) +"s;");
    methodIndex.addBody("$this->view->pages = ceil($total / $this->maxPerPage);");
    methodIndex.addBody("$this->view->currentPage = $page;");
    methodIndex.addBody("$this->view->total = $total;");
    methodIndex.addBody("return $this->view;");
    methodIndex.setDocblock(docblockIndex);
    this->code.addMethod(methodIndex);



    Method methodCreate;
    Docblock docblockCreate;
    docblockCreate.setShortDescription("Create");
    methodCreate.setName("createAction");
    methodCreate.setVisibility(Method::PUBLIC);
    methodCreate.isStatic(false);
    methodCreate.addBody("$" + this->lcFirst(this->model.getName()) +" = new " + this->model.getName() +"();");
    methodCreate.addBody("$this->view->" + this->lcFirst(this->model.getName()) +" = $" + this->lcFirst(this->model.getName()) +";");
    methodCreate.addBody("");
    methodCreate.addBody("//Views");
    methodCreate.addBody("$this->view->setTemplate(\"" + this->model.getModule().toLower() +"/" + this->model.getName().toLower() +"/form.tpl\");");
    methodCreate.addBody("return $this->view;");
    methodCreate.setDocblock(docblockCreate);
    this->code.addMethod(methodCreate);

    Method methodUpdate;
    Docblock docblockUpdate;
    docblockUpdate.setShortDescription("update");
    methodUpdate.setName("updateAction");
    methodUpdate.setVisibility(Method::PUBLIC);
    methodUpdate.isStatic(false);
    methodUpdate.addBody("try {");
    methodUpdate.addBody("\t$id" + this->model.getName() +" = $this->params()->fromRoute(\"id\", 0);");
    methodUpdate.addBody("\tif(!$id" + this->model.getName() +")");
    methodUpdate.addBody("\t\tthrow new \\Exception($this->i18n->translate('" + this->model.getName() +" not defined.'));");
    methodUpdate.addBody("");
    methodUpdate.addBody("\t$" + this->lcFirst(this->model.getName()) +"Query = new " + this->model.getName() +"Query($this->getAdatper());");
    methodUpdate.addBody("\t$" + this->lcFirst(this->model.getName()) +" = $" + this->lcFirst(this->model.getName()) +"Query->findByPkOrThrow($id" + this->model.getName() +", $this->i18n->translate(\"" + this->model.getName() +" not found.\"));");
    methodUpdate.addBody("");
    methodUpdate.addBody("\t//Views");
    methodUpdate.addBody("\t$this->view->" + this->lcFirst(this->model.getName()) +" = $" + this->lcFirst(this->model.getName()) +";");
    methodUpdate.addBody("\t$this->view->setTemplate(\"" + this->model.getModule().toLower() +"/" + this->model.getName().toLower() +"/form.tpl\");");
    methodUpdate.addBody("\treturn $this->view;");
    methodUpdate.addBody("} catch (\\Exception $e) {");
    methodUpdate.addBody("\t$this->flashMessenger()->addErrorMessage($e->getMessage());");
    methodUpdate.addBody("\t$this->redirect()->toRoute(null, array(");
    methodUpdate.addBody("\t\t'controller' => '" + this->model.getName().toLower() +" ',");
    methodUpdate.addBody("\t\t'action' =>  'index',");
    methodUpdate.addBody("\t));");
    methodUpdate.addBody("}");

    methodUpdate.addBody("//Views");
    methodUpdate.addBody("$this->view->setTemplate(\"" + this->model.getModule().toLower() +"/" + this->model.getName().toLower() +"/form.tpl\");");
    methodUpdate.addBody("return $this->view;");
    methodUpdate.setDocblock(docblockUpdate);
    this->code.addMethod(methodUpdate);



    Method methodSave;
    Docblock docblockSave;
    docblockSave.setShortDescription("Save");
    methodSave.setName("saveAction");
    methodSave.setVisibility(Method::PUBLIC);
    methodSave.isStatic(false);
    methodSave.addBody("$id" + this->model.getName() +" = $this->params()->fromPost(\"id\", 0);");
    methodSave.addBody("if($id" + this->model.getName() +")");
    methodSave.addBody("{");
    methodSave.addBody("\t$" + this->lcFirst(this->model.getName()) +"Query = new " + this->model.getName() +"Query($this->getAdatper());");
    methodSave.addBody("\t$" + this->lcFirst(this->model.getName()) +" = $" + this->lcFirst(this->model.getName()) +"Query->findByPkOrThrow($id" + this->model.getName() +", $this->i18n->translate(\"" + this->model.getName() +" not found.\"));");
    methodSave.addBody("}else{");
    methodSave.addBody("\t$" + this->lcFirst(this->model.getName()) +" = new " + this->model.getName() +"();");
    //if has status
    if(hasStatus)
        methodSave.addBody("\t$" + this->lcFirst(this->model.getName()) +"->setStatus(" + this->model.getName() +"::ENABLE);");

    methodSave.addBody("}");
    methodSave.addBody("");
    methodSave.addBody("$" + this->lcFirst(this->model.getName()) +"Catalog = new " + this->model.getName() +"Catalog($this->getAdatper());");
    methodSave.addBody("$" + this->lcFirst(this->model.getName()) +"Catalog->beginTransaction();");

    methodSave.addBody("try {");
    methodSave.addBody("\t" + this->model.getName() +" Factory::populate($" + this->lcFirst(this->model.getName()) +", $this->params()->fromPost());");
    methodSave.addBody("\t$" + this->lcFirst(this->model.getName()) +"Catalog->save($" + this->lcFirst(this->model.getName()) +");");
    if(this->model.getLog())
        methodSave.addBody("\t($id" + this->model.getName() +")?$this->newLog($" + this->lcFirst(this->model.getName()) +", Log::UPDATED):$this->newLog($" + this->lcFirst(this->model.getName()) +", Log::CREATED);");

    methodSave.addBody("\t$" + this->lcFirst(this->model.getName()) +"Catalog->commit();");
    methodSave.addBody("\t$this->flashMessenger()->addSuccessMessage('" + this->model.getName() +" Saved.');");
    methodSave.addBody("} catch (\\Exception $e) {");
    methodSave.addBody("\t$this->flashMessenger()->addErrorMessage($e->getMessage());");
    methodSave.addBody("\t$" + this->lcFirst(this->model.getName()) +"Catalog->rollback();");
    methodSave.addBody("}");

    methodSave.addBody("$this->redirect()->toRoute(null,array('controller'=>'" + this->model.getName().toLower() +"','action' => 'index',));");
    methodSave.addBody("return $this->view;");
    methodSave.setDocblock(docblockSave);
    this->code.addMethod(methodSave);


    if(hasStatus)
    {
        Method methodEnable;
        Docblock docblockEnable;
        docblockEnable.setShortDescription("Enable");
        methodEnable.setName("enableAction");
        methodEnable.setVisibility(Method::PUBLIC);
        methodEnable.isStatic(false);
        methodEnable.addBody("$" + this->lcFirst(this->model.getName()) +"Catalog = new " + this->model.getName() +"Catalog($this->getAdatper());");
        methodEnable.addBody("$" + this->lcFirst(this->model.getName()) +"Catalog->beginTransaction();");
        methodEnable.addBody("try {");
        methodEnable.addBody("\t$id" + this->model.getName() +" = $this->params()->fromRoute(\"id\", 0);");
        methodEnable.addBody("\tif(!$id" + this->model.getName() +")");
        methodEnable.addBody("\t\tthrow new \\Exception($this->i18n(\"" + this->model.getName() +" not defined.\"));");
        methodEnable.addBody("\t$" + this->lcFirst(this->model.getName()) +"Query = new " + this->model.getName() +"Query($this->getAdatper());");
        methodEnable.addBody("\t$" + this->lcFirst(this->model.getName()) +" = $" + this->lcFirst(this->model.getName()) +"Query->findByPkOrThrow($id" + this->model.getName() +", $this->i18n->translate(\"" + this->model.getName() +" not found.\"));");
        methodEnable.addBody("\t$" + this->lcFirst(this->model.getName()) +"->setStatus(" + this->model.getName() +"::ENABLE);");
        methodEnable.addBody("\t$" + this->lcFirst(this->model.getName()) +"Catalog->save($" + this->lcFirst(this->model.getName()) +");");
        methodEnable.addBody("\t$this->newLog($" + this->lcFirst(this->model.getName()) +", Log::ENABLED);");
        methodEnable.addBody("\t$" + this->lcFirst(this->model.getName()) +"Catalog->commit();");
        methodEnable.addBody("\t$this->flashMessenger()->addSuccessMessage('" + this->model.getName() +" has been enabled.');");
        methodEnable.addBody("} catch (\\Exception $e) {");
        methodEnable.addBody("\t$this->flashMessenger()->addErrorMessage($e->getMessage());");
        methodEnable.addBody("\t$" + this->lcFirst(this->model.getName()) +"    Catalog->rollback();");
        methodEnable.addBody("}");
        methodEnable.addBody("$this->redirect()->toRoute(null,array('controller'=>'" + this->model.getName().toLower() +"','action' => 'index',));");
        methodEnable.addBody("return $this->view;");
        methodEnable.setDocblock(docblockEnable);
        this->code.addMethod(methodEnable);

        Method methodDisable;
        Docblock docblockDisable;
        docblockDisable.setShortDescription("Disable");
        methodDisable.setName("disableAction");
        methodDisable.setVisibility(Method::PUBLIC);
        methodDisable.isStatic(false);
        methodDisable.addBody("$" + this->lcFirst(this->model.getName()) +"Catalog = new " + this->model.getName() +"Catalog($this->getAdatper());");
        methodDisable.addBody("$" + this->lcFirst(this->model.getName()) +"Catalog->beginTransaction();");
        methodDisable.addBody("try {");
        methodDisable.addBody("\t$id" + this->model.getName() +" = $this->params()->fromRoute(\"id\", 0);");
        methodDisable.addBody("\tif(!$id" + this->model.getName() +")");
        methodDisable.addBody("\t\tthrow new \\Exception($this->i18n(\"" + this->model.getName() +" not defined.\"));");
        methodDisable.addBody("\t$" + this->lcFirst(this->model.getName()) +"Query = new " + this->model.getName() +"Query($this->getAdatper());");
        methodDisable.addBody("\t$" + this->lcFirst(this->model.getName()) +" = $" + this->lcFirst(this->model.getName()) +"Query->findByPkOrThrow($id" + this->model.getName() +", $this->i18n->translate(\"" + this->model.getName() +" not found.\"));");
        methodDisable.addBody("\t$" + this->lcFirst(this->model.getName()) +"->setStatus(" + this->model.getName() +"::DISABLE);");
        methodDisable.addBody("\t$" + this->lcFirst(this->model.getName()) +"Catalog->save($" + this->lcFirst(this->model.getName()) +");");
        methodDisable.addBody("\t$this->newLog($" + this->lcFirst(this->model.getName()) +", Log::DISABLE);");
        methodDisable.addBody("\t$" + this->lcFirst(this->model.getName()) +"Catalog->commit();");
        methodDisable.addBody("\t$this->flashMessenger()->addSuccessMessage('" + this->model.getName() +" has been disabled.');");
        methodDisable.addBody("} catch (\\Exception $e) {");
        methodDisable.addBody("\t$this->flashMessenger()->addErrorMessage($e->getMessage());");
        methodDisable.addBody("\t$" + this->lcFirst(this->model.getName()) +"    Catalog->rollback();");
        methodDisable.addBody("}");
        methodDisable.addBody("$this->redirect()->toRoute(null,array('controller'=>'" + this->model.getName().toLower() +"','action' => 'index',));");
        methodDisable.addBody("return $this->view;");
        methodDisable.setDocblock(docblockDisable);
        this->code.addMethod(methodDisable);
    }
    Docblock docblock;
    docblock.setShortDescription(this->model.getName() + "Controller");
    docblock.setLongDescription("GeCo");
    docblock.addTag("autor", "isdarka");
    docblock.addTag("category", "Model");
    docblock.addTag("package", "Controller");
    docblock.addTag("copyright", "");
    docblock.addTag("license", "");
    docblock.addTag("created", QDateTime::currentDateTime().toString());
    docblock.addTag("version", "1.0");

    this->code.setDocblock(docblock);

    this->code.setNamespace(this->model.getModule() + "\\Controller");


}

GeCoBean ZfCRUD::getByExntendName(QString extend)
{
    GeCoBean model;
    foreach (model, this->models) {
        if(model.getName() == extend)
            return model;
    }
}

void ZfCRUD::write( QString str)
{
    QFile file(str);
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream out(&file);
        out << this->code.generate();

        // optional, as QFile destructor will already do it:
        file.close();
}
