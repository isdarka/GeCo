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
    QRegExp exp("([A-Z])");
    GeCoBean extra;
    this->code.setClassName(this->model.getName() + "Controller");
    if(!this->model.getExtend().isEmpty())
        extra = this->getByExntendName(this->model.getExtend());

    this->code.setExtends("BaseController");
    this->code.addUse("BaseProject\\Controller\\BaseController");



    this->code.addUse(this->model.getModule() + "\\Query\\" +this->model.getName() + "Query");
    this->code.addUse(this->model.getModule() + "\\Model\\Bean\\" +this->model.getName() + "");
    this->code.addUse(this->model.getModule() + "\\Model\\Catalog\\" +this->model.getName() + "Catalog");
    this->code.addUse(this->model.getModule() + "\\Model\\Factory\\" +this->model.getName() + "Factory");
    this->code.addUse(this->model.getModule() + "\\Model\\Exception\\" +this->model.getName() + "Exception");
    this->code.addUse("Query\\Exception\\QueryException");

    TableCatalog t;
    this->columns = t.getColumnsByTable(this->model.getTable());
    ColumnBean column;
    ColumnBean primaryKey;

    QStringList foreingKeys;
    foreingKeys.empty();
    bool hasStatus = false;
    foreach (column, this->columns) {
        if(column.getField() == "status")
            hasStatus = true;

        if(column.isPkAutoIncrement())
            primaryKey = column;

        if(column.isForeingKey())
        {
            QString entity = column.getField().mid(3, column.getField().size());
            foreingKeys.append(this->ucfirst(entity));
            //qDebug() << this->ucfirst(entity);
           // foreingKeys.append(this->ucfirst(column.getField()));
        }
    }
    foreingKeys.removeDuplicates();
//    QString nameFo;
//    foreach (nameFo, foreingKeys) {
//        qDebug() <<  nameFo;
//    }

    Method methodIndex;
    Docblock docblockIndex;
    docblockIndex.setShortDescription("Index");
    methodIndex.setName("indexAction");
    methodIndex.setVisibility(Method::PUBLIC);
    methodIndex.isStatic(false);
    methodIndex.addBody("$queryParams = $this->params()->fromQuery();");
    methodIndex.addBody("$" + this->lcFirst(this->model.getName()) +"Query = new " + this->model.getName() +"Query($this->getAdapter());");
    methodIndex.addBody("$" + this->lcFirst(this->model.getName()) +"Query->filter($queryParams);");
    methodIndex.addBody("$total = $" + this->lcFirst(this->model.getName()) +"Query->count();");
    methodIndex.addBody("$page = $this->params()->fromRoute(\"page\", 1);");
    methodIndex.addBody("$" + this->lcFirst(this->model.getName()) +"s = $" + this->lcFirst(this->model.getName()) +"Query->limit($this->maxPerPage)->offset(($page -1) * $this->maxPerPage)->find();");
    methodIndex.addBody("$this->setPaginator($total, $page, __METHOD__);");

    QString relation;
    GeCoBean relationBean;
    foreach (relation, foreingKeys) {
        relationBean = this->getByExntendName(relation);
        this->code.addUse(relationBean.getModule() + "\\Query\\" + relationBean.getName() + "Query");
        methodIndex.addBody("$" + this->lcFirst(relationBean.getName()) + "Query = new "+ relationBean.getName() + "Query($this->getAdapter());");
        methodIndex.addBody("$" + this->lcFirst(relationBean.getName()) + "Collection = $"+ this->lcFirst(relationBean.getName()) + "Query->find();");
    }

    methodIndex.addBody("");
    methodIndex.addBody("//Views");
    methodIndex.addBody("$this->view->" + this->lcFirst(this->model.getName()) +"s = $" + this->lcFirst(this->model.getName()) +"s;");
    methodIndex.addBody("$this->view->pages = ceil($total / $this->maxPerPage);");
    methodIndex.addBody("$this->view->currentPage = $page;");
    methodIndex.addBody("$this->view->total = $total;");
    methodIndex.addBody("$this->view->queryParams = $queryParams;");
    if(!foreingKeys.isEmpty())
        methodIndex.addBody("//Relations");
    foreach (relation, foreingKeys) {
        relationBean = this->getByExntendName(relation);
        methodIndex.addBody("$this->view->" + this->lcFirst(relationBean.getName()) + "Collection = $"+ this->lcFirst(relationBean.getName()) + "Collection;");
    }
    if(!foreingKeys.isEmpty())
        methodIndex.addBody("");
    if(hasStatus)
        methodIndex.addBody("$this->view->statuses = " + this->model.getName() + "::$statuses;");
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


    foreach (relation, foreingKeys) {
        relationBean = this->getByExntendName(relation);
        this->code.addUse(relationBean.getModule() + "\\Query\\" + relationBean.getName() + "Query");
        methodCreate.addBody("$" + this->lcFirst(relationBean.getName()) + "Query = new "+ relationBean.getName() + "Query($this->getAdapter());");
        methodCreate.addBody("$" + this->lcFirst(relationBean.getName()) + "Collection = $"+ this->lcFirst(relationBean.getName()) + "Query->find();");
    }
    if(!foreingKeys.isEmpty()){
    methodCreate.addBody("");
    methodCreate.addBody("//Relations");
    }
    foreach (relation, foreingKeys) {
        relationBean = this->getByExntendName(relation);
        methodCreate.addBody("$this->view->" + this->lcFirst(relationBean.getName()) + "Collection = $"+ this->lcFirst(relationBean.getName()) + "Collection;");
    }


    methodCreate.addBody("");    
    methodCreate.addBody("//Views");
    methodCreate.addBody("$this->view->setTemplate(\"" + this->lcFirst(this->ucfirst(this->model.getModule())).replace(exp, "-\\1").toLower() +"/" + this->lcFirst(this->ucfirst(this->model.getName())).replace(exp, "-\\1").toLower() +"/form.tpl\");");
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
    methodUpdate.addBody("\t\tthrow new " + this->model.getName() +"Exception($this->i18n->translate('" + this->model.getName() +" not defined'));");
    methodUpdate.addBody("");
    methodUpdate.addBody("\t$" + this->lcFirst(this->model.getName()) +"Query = new " + this->model.getName() +"Query($this->getAdapter());");
    methodUpdate.addBody("\t$" + this->lcFirst(this->model.getName()) +" = $" + this->lcFirst(this->model.getName()) +"Query->findByPkOrThrow($id" + this->model.getName() +", $this->i18n->translate(\"" + this->model.getName() +" not found\"));");


    foreach (relation, foreingKeys) {
        relationBean = this->getByExntendName(relation);
        this->code.addUse(relationBean.getModule() + "\\Query\\" + relationBean.getName() + "Query");
        methodUpdate.addBody("\t$" + this->lcFirst(relationBean.getName()) + "Query = new "+ relationBean.getName() + "Query($this->getAdapter());");
        methodUpdate.addBody("\t$" + this->lcFirst(relationBean.getName()) + "Collection = $"+ this->lcFirst(relationBean.getName()) + "Query->find();");
    }
    if(!foreingKeys.isEmpty()){
        methodUpdate.addBody("");
        methodUpdate.addBody("\t//Relations");
    }
    foreach (relation, foreingKeys) {
        relationBean = this->getByExntendName(relation);
        methodUpdate.addBody("\t$this->view->" + this->lcFirst(relationBean.getName()) + "Collection = $"+ this->lcFirst(relationBean.getName()) + "Collection;");
    }

    methodUpdate.addBody("");
    methodUpdate.addBody("\t//Views");
    methodUpdate.addBody("\t$this->view->" + this->lcFirst(this->model.getName()) +" = $" + this->lcFirst(this->model.getName()) +";");
    //methodUpdate.addBody("\treturn $this->view;");
    methodUpdate.addBody("} catch (" + this->model.getName() +"Exception $e) {");
    methodUpdate.addBody("\t$this->flashMessenger()->addErrorMessage($e->getMessage());");
    methodUpdate.addBody("\treturn $this->redirect()->toRoute(null, array(");
    methodUpdate.addBody("\t\t'controller' => '" + this->model.getName().toLower() +"',");
    methodUpdate.addBody("\t\t'action' =>  'index',");
    methodUpdate.addBody("\t));");
    methodUpdate.addBody("} catch (QueryException $e) {");
    methodUpdate.addBody("\t$this->flashMessenger()->addErrorMessage($e->getMessage());");
    methodUpdate.addBody("\treturn $this->redirect()->toRoute(null, array(");
    methodUpdate.addBody("\t\t'controller' => '" + this->model.getName().toLower() +"',");
    methodUpdate.addBody("\t\t'action' =>  'index',");
    methodUpdate.addBody("\t));");
    methodUpdate.addBody("}");

    methodUpdate.addBody("//Views");
    methodUpdate.addBody("$this->view->setTemplate(\"" + this->lcFirst(this->ucfirst(this->model.getModule())).replace(exp, "-\\1").toLower() +"/" + this->lcFirst(this->ucfirst(this->model.getName())).replace(exp, "-\\1").toLower() +"/form.tpl\");");
    methodUpdate.addBody("return $this->view;");
    methodUpdate.setDocblock(docblockUpdate);
    this->code.addMethod(methodUpdate);



    Method methodSave;
    Docblock docblockSave;
    docblockSave.setShortDescription("Save");
    methodSave.setName("saveAction");
    methodSave.setVisibility(Method::PUBLIC);
    methodSave.isStatic(false);
    methodSave.addBody("$id" + this->model.getName() +" = $this->params()->fromPost(\"" + this->lcFirst(this->ucfirst(primaryKey.getField())) +"\", 0);");
    methodSave.addBody("if($id" + this->model.getName() +")");
    methodSave.addBody("{");
    methodSave.addBody("\t$" + this->lcFirst(this->model.getName()) +"Query = new " + this->model.getName() +"Query($this->getAdapter());");
    methodSave.addBody("\t$" + this->lcFirst(this->model.getName()) +" = $" + this->lcFirst(this->model.getName()) +"Query->findByPkOrThrow($id" + this->model.getName() +", $this->i18n->translate(\"" + this->model.getName() +" not found\"));");
    methodSave.addBody("}else{");
    methodSave.addBody("\t$" + this->lcFirst(this->model.getName()) +" = new " + this->model.getName() +"();");
    //if has status
    if(hasStatus)
        methodSave.addBody("\t$" + this->lcFirst(this->model.getName()) +"->setStatus(" + this->model.getName() +"::ENABLE);");

    methodSave.addBody("}");
    methodSave.addBody("");
    methodSave.addBody("$" + this->lcFirst(this->model.getName()) +"Catalog = new " + this->model.getName() +"Catalog($this->getAdapter());");
    methodSave.addBody("$" + this->lcFirst(this->model.getName()) +"Catalog->beginTransaction();");

    methodSave.addBody("try {");
    methodSave.addBody("\t" + this->model.getName() +"Factory::populate($" + this->lcFirst(this->model.getName()) +", $this->params()->fromPost());");
    methodSave.addBody("\t$" + this->lcFirst(this->model.getName()) +"Catalog->save($" + this->lcFirst(this->model.getName()) +");");
    if(this->model.getLog())
        methodSave.addBody("\t($id" + this->model.getName() +")?$this->newLog($" + this->lcFirst(this->model.getName()) +", Log::UPDATED):$this->newLog($" + this->lcFirst(this->model.getName()) +", Log::CREATED);");

    methodSave.addBody("\t$" + this->lcFirst(this->model.getName()) +"Catalog->commit();");
    methodSave.addBody("\t$this->flashMessenger()->addSuccessMessage('" + this->model.getName() +" Saved.');");
    methodSave.addBody("} catch (" + this->model.getName() +"Exception $e) {");
    methodSave.addBody("\t$this->flashMessenger()->addErrorMessage($e->getMessage());");
    methodSave.addBody("\t$" + this->lcFirst(this->model.getName()) +"Catalog->rollback();");
    methodSave.addBody("}");

    methodSave.addBody("return $this->redirect()->toRoute(null,array('controller'=>'" + this->lcFirst(this->ucfirst(this->model.getName())).replace(exp, "-\\1").toLower() +"','action' => 'index',));");
//    methodSave.addBody("return $this->view;");
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
        methodEnable.addBody("$" + this->lcFirst(this->model.getName()) +"Catalog = new " + this->model.getName() +"Catalog($this->getAdapter());");
        methodEnable.addBody("$" + this->lcFirst(this->model.getName()) +"Catalog->beginTransaction();");
        methodEnable.addBody("try {");
        methodEnable.addBody("\t$id" + this->model.getName() +" = $this->params()->fromRoute(\"id\", 0);");
        methodEnable.addBody("\tif(!$id" + this->model.getName() +")");
        methodEnable.addBody("\t\tthrow new " + this->model.getName() +"Exception($this->i18n->translate(\"" + this->model.getName() +" not defined\"));");
        methodEnable.addBody("\t$" + this->lcFirst(this->model.getName()) +"Query = new " + this->model.getName() +"Query($this->getAdapter());");
        methodEnable.addBody("\t$" + this->lcFirst(this->model.getName()) +" = $" + this->lcFirst(this->model.getName()) +"Query->findByPkOrThrow($id" + this->model.getName() +", $this->i18n->translate(\"" + this->model.getName() +" not found\"));");
        methodEnable.addBody("\t$" + this->lcFirst(this->model.getName()) +"->setStatus(" + this->model.getName() +"::ENABLE);");
        methodEnable.addBody("\t$" + this->lcFirst(this->model.getName()) +"Catalog->save($" + this->lcFirst(this->model.getName()) +");");
        if(this->model.getLog())
            methodEnable.addBody("\t$this->newLog($" + this->lcFirst(this->model.getName()) +", " + this->model.getName() +"::ENABLE);");
        methodEnable.addBody("\t$" + this->lcFirst(this->model.getName()) +"Catalog->commit();");
        methodEnable.addBody("\t$this->flashMessenger()->addSuccessMessage('" + this->model.getName() +" has been enabled.');");
        methodEnable.addBody("} catch (" + this->model.getName() +"Exception $e) {");
        methodEnable.addBody("\t$this->flashMessenger()->addErrorMessage($e->getMessage());");
        methodEnable.addBody("\t$" + this->lcFirst(this->model.getName()) +"Catalog->rollback();");
        methodEnable.addBody("}");
        methodEnable.addBody("return $this->redirect()->toRoute(null,array('controller'=>'" + this->lcFirst(this->ucfirst(this->model.getName())).replace(exp, "-\\1").toLower() +"','action' => 'index',));");
//        methodEnable.addBody("return $this->view;");
        methodEnable.setDocblock(docblockEnable);
        this->code.addMethod(methodEnable);

        Method methodDisable;
        Docblock docblockDisable;
        docblockDisable.setShortDescription("Disable");
        methodDisable.setName("disableAction");
        methodDisable.setVisibility(Method::PUBLIC);
        methodDisable.isStatic(false);
        methodDisable.addBody("$" + this->lcFirst(this->model.getName()) +"Catalog = new " + this->model.getName() +"Catalog($this->getAdapter());");
        methodDisable.addBody("$" + this->lcFirst(this->model.getName()) +"Catalog->beginTransaction();");
        methodDisable.addBody("try {");
        methodDisable.addBody("\t$id" + this->model.getName() +" = $this->params()->fromRoute(\"id\", 0);");
        methodDisable.addBody("\tif(!$id" + this->model.getName() +")");
        methodDisable.addBody("\t\tthrow new " + this->model.getName() +"Exception($this->i18n->translate(\"" + this->model.getName() +" not defined\"));");
        methodDisable.addBody("\t$" + this->lcFirst(this->model.getName()) +"Query = new " + this->model.getName() +"Query($this->getAdapter());");
        methodDisable.addBody("\t$" + this->lcFirst(this->model.getName()) +" = $" + this->lcFirst(this->model.getName()) +"Query->findByPkOrThrow($id" + this->model.getName() +", $this->i18n->translate(\"" + this->model.getName() +" not found\"));");
        methodDisable.addBody("\t$" + this->lcFirst(this->model.getName()) +"->setStatus(" + this->model.getName() +"::DISABLE);");
        methodDisable.addBody("\t$" + this->lcFirst(this->model.getName()) +"Catalog->save($" + this->lcFirst(this->model.getName()) +");");
        if(this->model.getLog())
            methodDisable.addBody("\t$this->newLog($" + this->lcFirst(this->model.getName()) +", " + this->model.getName() +"::DISABLE);");
        methodDisable.addBody("\t$" + this->lcFirst(this->model.getName()) +"Catalog->commit();");
        methodDisable.addBody("\t$this->flashMessenger()->addSuccessMessage('" + this->model.getName() +" has been disabled.');");
        methodDisable.addBody("} catch (" + this->model.getName() +"Exception $e) {");
        methodDisable.addBody("\t$this->flashMessenger()->addErrorMessage($e->getMessage());");
        methodDisable.addBody("\t$" + this->lcFirst(this->model.getName()) +"Catalog->rollback();");
        methodDisable.addBody("}");
        methodDisable.addBody("return $this->redirect()->toRoute(null,array('controller'=>'" + this->lcFirst(this->ucfirst(this->model.getName())).replace(exp, "-\\1").toLower() +"','action' => 'index',));");
//        methodDisable.addBody("return $this->view;");
        methodDisable.setDocblock(docblockDisable);
        this->code.addMethod(methodDisable);
    }

    if(this->model.getLog())
    {
        this->code.addUse("Model\\Bean\\AbstractBean");
        GeCoBean parentLog;
        parentLog = this->getByExntendName("Log");
        this->code.addUse(parentLog.getModule() + "\\Model\\Bean\\" +parentLog.getName());

        GeCoBean Log;
        Log = this->getByExntendName(this->model.getLogName());




        this->code.addUse(Log.getModule() + "\\Query\\" +Log.getName()  + "Query");
        this->code.addUse(Log.getModule() + "\\Model\\Bean\\" +Log.getName()  + "");
        this->code.addUse(Log.getModule() + "\\Model\\Catalog\\" +Log.getName() + "Catalog");
        this->code.addUse(Log.getModule()+ "\\Model\\Factory\\" +Log.getName() + "Factory");


        //History
        Method methodHistory;
        Docblock History;
        History.setShortDescription("History");
        methodHistory.setName("historyAction");
        methodHistory.setVisibility(Method::PUBLIC);
        methodHistory.isStatic(false);
        methodHistory.addBody("try {");
        methodHistory.addBody("\t$id" + this->model.getName() +" = $this->params()->fromRoute(\"id\", 0);");
        methodHistory.addBody("\t$page = $this->params()->fromRoute(\"page\", 1);");
        methodHistory.addBody("\tif(!$id" + this->model.getName() +" )");
        methodHistory.addBody("\t\tthrow new " + this->model.getName() +"Exception($this->i18n->translate(\"" + this->model.getName() +" not defined\"));");

        methodHistory.addBody("\t$userQuery = new UserQuery($this->getAdapter());");
        methodHistory.addBody("\t$users = $userQuery->find();");

        methodHistory.addBody("\t$" + this->lcFirst(this->model.getName()) +"Query = new " + this->model.getName() +"Query($this->getAdapter());");
        methodHistory.addBody("\t$" + this->lcFirst(this->model.getName()) +" = $" + this->lcFirst(this->model.getName()) +"Query->findByPkOrThrow($id" + this->model.getName() +", $this->i18n->translate(\"" + this->model.getName() +" not found\"));");
        methodHistory.addBody("\t$" + this->lcFirst(this->model.getName()) +"LogQuery = new " + this->model.getName() +"LogQuery($this->getAdapter());");
        methodHistory.addBody("\t$" + this->lcFirst(this->model.getName()) +"LogQuery->whereAdd(" + this->model.getName() +"Log::" + primaryKey.getField().toUpper() +", $" + this->lcFirst(this->model.getName()) +"->get" + this->ucfirst(primaryKey.getField()) +"());");
        methodHistory.addBody("\t$total = $" + this->lcFirst(this->model.getName()) +"LogQuery->count();");

        methodHistory.addBody("\t$" + this->lcFirst(this->model.getName()) +"LogQuery->addDescendingOrderBy(" + this->model.getName() +"Log::" + primaryKey.getField().toUpper() +"_LOG );");
        methodHistory.addBody("\t$" + this->lcFirst(this->model.getName()) +"LogQuery->limit($this->maxPerPage)->offset(($page -1) * $this->maxPerPage);");
        methodHistory.addBody("\t$" + this->lcFirst(this->model.getName()) +"Logs = $" + this->lcFirst(this->model.getName()) +"LogQuery->find();");
        methodHistory.addBody("\t$" + this->lcFirst(this->model.getName()) +"Query = new " + this->model.getName() +"Query($this->getAdapter());");
        methodHistory.addBody("\t$" + this->lcFirst(this->model.getName()) +"s = $" + this->lcFirst(this->model.getName()) +"Query->find();");
        methodHistory.addBody("\t$this->setPaginator($total, $page, __METHOD__);");

        methodHistory.addBody("");
        methodHistory.addBody("\t //Views");
        methodHistory.addBody("\t$this->view->" + this->lcFirst(this->model.getName()) +"Logs = $" + this->lcFirst(this->model.getName()) +"Logs;");
        methodHistory.addBody("\t$this->view->" + this->lcFirst(this->model.getName()) +"s = $" + this->lcFirst(this->model.getName()) +"s;");
        methodHistory.addBody("\t$this->view->users = $users;");

        this->code.addUse("Core\\Query\\UserQuery");
        methodHistory.addBody("} catch (" + this->model.getName() +"Exception $e) {");
        methodHistory.addBody("\t$this->flashMessenger()->addErrorMessage($e->getMessage());");
        methodHistory.addBody("\treturn $this->redirect()->toRoute(null,array('controller'=>'" + this->lcFirst(this->ucfirst(this->model.getName())).replace(exp, "-\\1").toLower() +"','action' => 'index',));");
        methodHistory.addBody("}");
        methodHistory.addBody("return $this->view;");
        methodHistory.setDocblock(History);
        this->code.addMethod(methodHistory);

        //LOG
        Method methodLog;
        Docblock dockblockLog;
        dockblockLog.setShortDescription("Log");
        methodLog.setName("newLog");
        methodLog.setVisibility(Method::PRIVATE);
        methodLog.isStatic(false);

        methodLog.addParam("bean", "AbstractBean");
        methodLog.addParam("event");
        methodLog.addParam("note = \"\"");


        methodLog.addBody("$" + this->lcFirst(this->model.getName()) +"LogCatalog = new " + this->model.getName() +"LogCatalog($this->getAdapter());");
        methodLog.addBody("$date = new \\DateTime(\"now\");");
        methodLog.addBody("$" + this->lcFirst(this->model.getName()) +"Log = " + this->model.getName() +"LogFactory::createFromArray(array(");
        methodLog.addBody("\t" + this->model.getName() +"Log::" + primaryKey.getField().toUpper() + " => $bean->get" + this->ucfirst(primaryKey.getField()) + "(),");
        methodLog.addBody("\t" + this->model.getName() +"Log::ID_USER => $this->getUser()->getIdUser(),");
        methodLog.addBody("\t" + this->model.getName() +"Log::EVENT => $event,");
        methodLog.addBody("\t" + this->model.getName() +"Log::NOTE => $note,");
        methodLog.addBody("\t" + this->model.getName() +"Log::TIMESTAMP => $date->format(\\DateTime::W3C),");
        methodLog.addBody("\t)");

        methodLog.addBody(");");
        methodLog.addBody("$" + this->lcFirst(this->model.getName()) +"LogCatalog->save($" + this->lcFirst(this->model.getName()) +"Log);");
        methodLog.setDocblock(dockblockLog);
        this->code.addMethod(methodLog);
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
