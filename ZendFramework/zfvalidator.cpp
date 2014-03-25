#include "zfvalidator.h"

//ZfValidator::ZfValidator()
//{
//}



ZfValidator::ZfValidator(GeCoBean model, QVector<GeCoBean> models)
{
    this->model = model;
    this->models = models;

}



QString ZfValidator::ucfirst(QString str)
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

QString ZfValidator::lcFirst(QString str)
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

void ZfValidator::generate()
{

    GeCoBean extra;
    bool hasStatus = false;
    this->code.setClassName(this->model.getName() + "Validator");
    if(!this->model.getExtend().isEmpty())
    {
        extra = this->getByExntendName(this->model.getExtend());
        this->code.setExtends(extra.getName() + "Validator");
    }else
        this->code.setExtends("AbstractValidator");

    TableCatalog t;
    this->columns = t.getColumnsByTable(this->model.getTable());
    ColumnBean column;







    //__construct
    Method construct;
    Docblock docblockconstruct;
    docblockconstruct.setShortDescription("Validate constructor");
    docblockconstruct.addTag("param", this->model.getName() + " " + this->ucfirst(this->model.getName()));
    construct.setName("__construct");
    construct.addParam(this->lcFirst(this->model.getName()), this->model.getName());
    construct.setVisibility(Method::PUBLIC);
    construct.addBody("parent::__construct($" + this->lcFirst(this->model.getName()) + ");");
    construct.setDocblock(docblockconstruct);
    this->code.addMethod(construct);
    this->code.addUse(this->model.getModule() + "\\Model\\Bean\\" + this->model.getName());

    //validate
    Method validate;
    Docblock docblockValidate;
    docblockValidate.setShortDescription("Validate Object " + this->model.getName());
    //docblockValidate.addTag("return", this->model.getName());
    validate.setName("get" + this->model.getName());
    validate.setVisibility(Method::PUBLIC);
    if(!this->model.getExtend().isEmpty())
        validate.addBody("parent::validate();");

    foreach (column, this->columns) {
        if(!column.isPkAutoIncrement())
            validate.addBody("$this->validate" + this->ucfirst(column.getField()) + "();");
    }
    validate.setDocblock(docblockValidate);
    this->code.addMethod(validate);

    //getBean
    Method getBean;
    Docblock docblockGetBean;
    docblockGetBean.setShortDescription("Get " + this->model.getName());
    docblockGetBean.addTag("return", this->model.getName());
    getBean.setName("get" + this->model.getName());
    getBean.setVisibility(Method::PUBLIC);
    getBean.addBody("return $this->bean;");
    getBean.setDocblock(docblockGetBean);
    this->code.addMethod(getBean);




    foreach (column, this->columns) {
        if(!column.isPkAutoIncrement())
        {
            Method method;
            Docblock docblock;
            docblock.setShortDescription("Validate " + this->lcFirst(this->ucfirst(column.getField())) + " property");


            method.setDocblock(docblock);
            method.setName("validate" + (this->ucfirst(column.getField())));
            method.setVisibility(Method::PUBLIC);

            if(!column.getNull())
            {
                method.addBody("$validator = $this->getNotEmptyValidator();");
                method.addBody("$validator->isValid($this->get" + this->model.getName() + "()->get" + this->ucfirst(column.getField())+ "());");
                method.addBody("if($validator->getMessages())");
                method.addBody("\t$this->messages = array_merge($this->messages, $validator->getMessages());");
            }
            //if(column.)
    //        if(column.isDate())
    //            method.addParam(this->lcFirst(this->ucfirst(column.getField())),"Zend_Date");
    //        else if(column.isTime())
    //            method.addParam(this->lcFirst(this->ucfirst(column.getField())),"Zend_Date");
    //        else
    //            method.addParam(this->lcFirst(this->ucfirst(column.getField())));

    //        method.addBody("$this->" + this->lcFirst(this->ucfirst(column.getField())) + " = $" + this->lcFirst(this->ucfirst(column.getField())) + ";");
            method.addBody("return $this;");
            this->code.addMethod(method);
        }
    }





       Docblock docblock;
       docblock.setShortDescription(this->model.getName() + "Validator");
       docblock.setLongDescription("GeCo");
       docblock.addTag("author", "isdarka");
       docblock.addTag("category", "Validator");
       docblock.addTag("package", "Validator");
       docblock.addTag("copyright", "");
       docblock.addTag("license", "");
       docblock.addTag("created", QDateTime::currentDateTime().toString());
       docblock.addTag("version", "1.0");

       this->code.setDocblock(docblock);

       this->code.setNamespace(this->model.getModule() + "\\Validator");



}

GeCoBean ZfValidator::getByExntendName(QString extend)
{
    GeCoBean model;
    foreach (model, this->models) {
        if(model.getName() == extend)
            return model;
    }
}

void ZfValidator::write( QString str)
{
    QFile file(str);
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream out(&file);
        out << this->code.generate();

        // optional, as QFile destructor will already do it:
        file.close();
}

