#include "zfview.h"


ZfView::ZfView(GeCoBean model, QVector<GeCoBean> models)
{
    this->model = model;
    this->models = models;

}



QString ZfView::ucfirst(QString str)
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

QString ZfView::lcFirst(QString str)
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

QString ZfView::generateIndex()
{
    QString index;

    index.append("<fieldset>\n");
    index.append("\t<legend>{$i18n->translate(\"").append(this->model.getName()).append("\")} <a href=\"{$baseUrl}/" + this->model.getModule().toLower() +"/" + this->model.getName().toLower() +"/create\" class=\"btn btn-success pull-right\">{$i18n->translate(\"New " + this->model.getName() +"\")}</a></legend>\n");
        index.append("\t<div class=\"table-responsive\">\n");
        index.append("\t<form action=\"{$baseUrl}/" + this->model.getModule().toLower() +"/" + this->model.getName().toLower() +"/index\" method=\"get\">\n");

            index.append("\t\t<table class=\"table table-bordered table-condensed table-hover\">\n");

                index.append("\t").append("\t").append("\t").append("<thead>").append("\n");


                    //FILTER
                    index.append("\t").append("\t").append("\t").append("\t").append("<tr class=\"well\">").append("\n");

                    TableCatalog t;
                    this->columns = t.getColumnsByTable(this->model.getTable());
                    ColumnBean column;
//                    bool hasStatus = false;
                    foreach (column, this->columns) {
                        index.append("\t").append("\t").append("\t").append("\t").append("\t").append("<th><input type=\"text\" class=\"form-control\" name=\"" + column.getField() + "\" placeholder=\"{$i18n->translate('" + column.getField().toUpper() + "')}\" value=\"{$queryParams['" + column.getField() + "']}\"></th>").append("\n");
                    }
                    index.append("\t").append("\t").append("\t").append("\t").append("\t").append("<th class=\"col-xs-2\"><button type=\"submit\" class=\"form-control btn btn-default \">{$i18n->translate(\"Filter\")}</button></th>").append("\n");
                    index.append("\t").append("\t").append("\t").append("\t").append("</tr>").append("\n");

                //END FILTER
                    index.append("\t").append("\t").append("\t").append("\t").append("<tr class=\"well\">").append("\n");

//                    TableCatalog t;
//                    this->columns = t.getColumnsByTable(this->model.getTable());
//                    ColumnBean column;
                    bool hasStatus = false;
                    foreach (column, this->columns) {
                        if(column.getField() == "status")
                            hasStatus = true;

                        index.append("\t").append("\t").append("\t").append("\t").append("\t").append("<th>{$i18n->translate(\"").append(this->ucfirst(column.getField())).append("\")}</th>").append("\n");




                    }
                    index.append("\t").append("\t").append("\t").append("\t").append("\t").append("<th>{$i18n->translate(\"Actions\")}</th>").append("\n");
                    index.append("\t").append("\t").append("\t").append("\t").append("</tr>").append("\n");
                index.append("\t").append("\t").append("\t").append("</thead>").append("\n");

                index.append("\t").append("\t").append("\t").append("<tbody>").append("\n");
                index.append("\t").append("\t").append("\t").append("\t").append("{foreach $").append(this->lcFirst(this->model.getName())).append("s as $").append(this->lcFirst(this->model.getName())).append("}").append("\n");
                    index.append("\t").append("\t").append("\t").append("\t").append("<tr>").append("\n");

                    foreach (column, this->columns) {
                        if(hasStatus && column.getField() == "status")
                            index.append("\t").append("\t").append("\t").append("\t").append("\t").append("<td>{$").append(this->lcFirst(this->model.getName())).append("->getStatusString()}</td>").append("\n");
                        else
                            index.append("\t").append("\t").append("\t").append("\t").append("\t").append("<td>{$").append(this->lcFirst(this->model.getName())).append("->get").append(this->ucfirst(column.getField())).append("()}</td>").append("\n");
                    }

//Actions
                    index.append("\t").append("\t").append("\t").append("\t").append("\t").append("<td>").append("\n");
                    index.append("\t").append("\t").append("\t").append("\t").append("\t").append("\t").append("<div class=\"btn-group\">").append("\n");
                    index.append("\t").append("\t").append("\t").append("\t").append("\t").append("\t")
                            .append("<a href=\"{url module=" + this->model.getModule().toLower()  +" controller=" + this->model.getName().toLower()  +" action=update id=$" + this->lcFirst(this->model.getName())+ "->getId" + this->model.getName()+"()}\" class=\"btn btn-default\" data-toggle=\"tooltip\" title=\"{$i18n->translate('Edit')}\"><span class=\"fa fa-pencil\"></span></a>")
                            .append("\n");
//<a href="{url module=core controller=user action=update idUser=$user->getIdUser()}" class="btn btn-default" data-toggle="tooltip" title="{$i18n->translate('Edit')}"><span class="fa fa-pencil"></span></a>

                    if(hasStatus)
                    {
                        index.append("\t").append("\t").append("\t").append("\t").append("\t").append("\t").append("\t")
                                .append("{if $" + this->lcFirst(this->model.getName()) + "->isEnabled()}")
                                .append("\n");

                        index.append("\t").append("\t").append("\t").append("\t").append("\t").append("\t").append("\t").append("\t")
                                .append("<a href=\"{url module=" + this->model.getModule().toLower()  +" controller=" + this->model.getName().toLower()  +" action=disable id=$" + this->lcFirst(this->model.getName())+ "->getId" + this->model.getName()+"()}\" class=\"btn btn-default\" data-toggle=\"tooltip\" title=\"{$i18n->translate('Disable')}\"><span class=\"fa fa-times-circle-o\"></span></a>")
                                .append("\n");

                        index.append("\t").append("\t").append("\t").append("\t").append("\t").append("\t").append("\t")
                                .append("{else if $" + this->lcFirst(this->model.getName()) + "->isDisabled()}")
                                .append("\n");

                        index.append("\t").append("\t").append("\t").append("\t").append("\t").append("\t").append("\t").append("\t")
                                .append("<a href=\"{url module=" + this->model.getModule().toLower()  +" controller=" + this->model.getName().toLower()  +" action=enable id=$" + this->lcFirst(this->model.getName())+ "->getId" + this->model.getName()+"()}\" class=\"btn btn-default\" data-toggle=\"tooltip\" title=\"{$i18n->translate('Enable')}\"><span class=\"fa fa-check-circle-o\"></span></a>")
                                .append("\n");

                        index.append("\t").append("\t").append("\t").append("\t").append("\t").append("\t").append("\t")
                                .append("{/if}")
                                .append("\n");
                    }

                    if(this->model.getLog())
                    {
                        index.append("\t").append("\t").append("\t").append("\t").append("\t").append("\t")
                                .append("<a href=\"{url module=" + this->model.getModule().toLower()  +" controller=" + this->model.getName().toLower()  +" action=history id=$" + this->lcFirst(this->model.getName())+ "->getId" + this->model.getName()+"()}\" class=\"btn btn-default\" data-toggle=\"tooltip\" title=\"{$i18n->translate('History')}\"><span class=\"fa fa-book\"></span></a>")
                                .append("\n");
    //<a href="{url module=core controller=user action=update idUser=$user->getIdUser()}" class="btn btn-default" data-toggle="tooltip" title="{$i18n->translate('History')}"><span class="fa fa-book"></span></a>
                    }

                    index.append("\t").append("\t").append("\t").append("\t").append("\t").append("\t").append("</div>").append("\n");
                    index.append("\t").append("\t").append("\t").append("\t").append("\t").append("</td>").append("\n");


                    index.append("\t").append("\t").append("\t").append("\t").append("</tr>").append("\n");
                index.append("\t").append("\t").append("\t").append("\t").append("{/foreach}").append("\n");
                index.append("\t").append("\t").append("\t").append("</tbody>").append("\n");

            index.append("\t\t</table>\n");
            index.append("\t\t</form>\n");

        index.append("\t</div>\n");
    index.append("</fieldset>\n");

    return index;
}


QString ZfView::generateForm()
{

    TableCatalog t;
    this->columns = t.getColumnsByTable(this->model.getTable());
    ColumnBean column;
    ColumnBean primaryKey;
    bool hasStatus = false;
    foreach (column, this->columns) {
        if(column.getField() == "status")
            hasStatus = true;
        if(column.isPkAutoIncrement())
            primaryKey = column;
    }

    QString index;

    index.append("<fieldset>\n");
    index.append("\t<legend>{$i18n->translate(\"").append(this->model.getName()).append("\")}</legend>\n");
    index.append("\t<form class=\"form-horizontal validate\" method=\"post\" action=\"{$baseUrl}/" + this->model.getModule().toLower() + "/" + this->model.getName().toLower() + "/save\">\n");
    index.append("\t\t<input type=\"hidden\" name=\"" + this->lcFirst(this->ucfirst(primaryKey.getField()))  +"\" id=\"" + this->lcFirst(this->ucfirst(primaryKey.getField())) +"\" value=\"{$" + this->lcFirst(this->model.getName()) +"->get" + this->ucfirst(primaryKey.getField()) + "()}\">\n");
    foreach (column, this->columns) {
        if(column.getField() != primaryKey.getField())
        {
            if(column.getField() != "status"){

                index.append("\t").append("\t");
                index.append("<div class=\"form-group\">").append("\n");
                index.append("\t\t\t").append("<label for=\"" + this->lcFirst(column.getField()) + "\" class=\"col-sm-2 control-label\">{$i18n->translate(\"" + this->ucfirst(column.getField()) + "\")}</label>").append("\n");
                index.append("\t\t\t").append("<div class=\"col-sm-10\">").append("\n");

                if(column.isDate())
                    index.append("\t\t\t\t").append("<input type=\"text\" class=\"form-control required datepicker\" id=\"" + this->lcFirst(this->ucfirst(primaryKey.getField())) +"\" name=\"" + column.getField() + "\" value=\"{$" + this->lcFirst(this->model.getName()) +"->get" + this->ucfirst(column.getField()) +"()}\">").append("\n");
                else if (column.isNumber())
                    index.append("\t\t\t\t").append("<input type=\"text\" class=\"form-control required int \" id=\"" + this->lcFirst(this->ucfirst(primaryKey.getField())) +"\" name=\"" + column.getField() + "\" value=\"{$" + this->lcFirst(this->model.getName()) +"->get" + this->ucfirst(column.getField()) +"()}\" >").append("\n");
                else if (column.isFloat())
                    index.append("\t\t\t\t").append("<input type=\"text\" class=\"form-control required float\" id=\"" + this->lcFirst(this->ucfirst(primaryKey.getField())) +"\" name=\"" + column.getField() + "\" value=\"{$" + this->lcFirst(this->model.getName()) +"->get" + this->ucfirst(column.getField()) +"()}\" >").append("\n");
                else if (column.isTime())
                    index.append("\t\t\t\t").append("<input type=\"text\" class=\"form-control required timepicker\" id=\"" + this->lcFirst(this->ucfirst(primaryKey.getField())) +"\" name=\"" + column.getField() + "\" value=\"{$" + this->lcFirst(this->model.getName()) +"->get" + this->ucfirst(column.getField()) +"()}\" >").append("\n");
                else if (column.isLongText())
                    index.append("\t\t\t\t").append("<textarea class=\"form-control required \" id=\"" + this->lcFirst(this->ucfirst(primaryKey.getField())) +"\" name=\"" + column.getField() + "\" >{$" + this->lcFirst(this->model.getName()) +"->get" + this->ucfirst(column.getField()) +"()}\"</textarea>").append("\n");
                else
                    index.append("\t\t\t\t").append("<input type=\"text\" class=\"form-control required\" id=\"" + this->lcFirst(this->ucfirst(primaryKey.getField())) +"\" name=\"" + column.getField() + "\" value=\"{$" + this->lcFirst(this->model.getName()) +"->get" + this->ucfirst(column.getField()) +"()}\">").append("\n");
                index.append("\t\t\t").append("</div>").append("\n");
                index.append("\t\t</div>").append("\n");



            }

        }
    }

    index.append("\t\t<div class=\"form-group\">").append("\n");
    index.append("\t\t\t<div class=\"col-sm-offset-2 col-sm-10\">").append("\n");
    index.append("\t\t\t<a href=\"{url module=" + this->model.getModule().toLower() + " controller=" + this->model.getName().toLower() + " action=index}\" class=\"btn btn-default\">{$i18n->translate(\"Cancel\")}</a>").append("\n");
    index.append("\t\t\t<button type=\"submit\" class=\"btn btn-primary\">{$i18n->translate(\"Save\")}</button>").append("\n");
    index.append("\t\t\t</div>").append("\n");
    index.append("\t\t</div>").append("\n");


    index.append("\t</form>\n");
    index.append("</fieldset>\n");

    return index;
}


QString ZfView::generateHistory()
{
    TableCatalog t;
    this->columns = t.getColumnsByTable(this->model.getTable());
    ColumnBean column;
    ColumnBean primaryKey;
    bool hasStatus = false;
    foreach (column, this->columns) {
        if(column.getField() == "status")
            hasStatus = true;
        if(column.isPkAutoIncrement())
            primaryKey = column;
    }

    QString index;

    index.append("<fieldset>\n");
    index.append("\t<legend>{$i18n->translate(\"History\")}</legend>\n");
    index.append("\t<div class=\"table-responsive\">\n");
    index.append("\t\t<table class=\"table table-bordered table-condensed table-hover\">").append("\n");
    index.append("\t\t\t<thead>").append("\n");
    index.append("\t\t\t\t<tr class=\"well\">").append("\n");
    index.append("\t\t\t\t\t<th>{$i18n->translate(\"Username\")}</th>").append("\n");
    index.append("\t\t\t\t\t<th>{$i18n->translate(\"Event\")}</th>").append("\n");
    index.append("\t\t\t\t\t<th>{$i18n->translate(\"Date\")}</th>").append("\n");
    index.append("\t\t\t\t\t<th>{$i18n->translate(\"Note\")}</th>").append("\n");
    index.append("\t\t\t\t</tr>").append("\n");
    index.append("\t\t\t</thead>").append("\n");
    index.append("\t\t\t<tbody>").append("\n");
    index.append("\t\t\t\t{foreach $" + this->lcFirst(this->ucfirst(this->model.getName()))+ "Logs as $" + this->lcFirst(this->ucfirst(this->model.getName()))+ "Log}").append("\n");
    index.append("\t\t\t\t{assign var=\"user\" value=$users->getByPK($" + this->lcFirst(this->ucfirst(this->model.getName()))+ "Log->getIdUser())}").append("\n");

    index.append("\t\t\t\t\t<tr>").append("\n");
    index.append("\t\t\t\t\t\t<td>{$user->getFullName()}</td>").append("\n");
    index.append("\t\t\t\t\t\t<td>{$" + this->lcFirst(this->ucfirst(this->model.getName()))+ "Log->getEventString()}</td>").append("\n");
    index.append("\t\t\t\t\t\t<td>{$" + this->lcFirst(this->ucfirst(this->model.getName()))+ "Log->getTimestamp()}</td>").append("\n");
    index.append("\t\t\t\t\t\t<td>{$" + this->lcFirst(this->ucfirst(this->model.getName()))+ "Log->getNote()}</td>").append("\n");
    index.append("\t\t\t\t\t</tr>").append("\n");
    index.append("\t\t\t\t{/foreach}").append("\n");
    index.append("\t\t\t</tbody>").append("\n");
    index.append("\t\t</table>").append("\n");
    index.append("\t</div>").append("\n");
    index.append("\t<a href=\"{url module=" + this->model.getModule().toLower() + " controller=" + this->model.getName().toLower() + " action=index}\" class=\"btn btn-default\">{$i18n->translate('Go Back')}</a>").append("\n");
    index.append("</fieldset>\n");

    return index;
}
GeCoBean ZfView::getByExntendName(QString extend)
{
    GeCoBean model;
    foreach (model, this->models) {
        if(model.getName() == extend)
            return model;
    }
}

void ZfView::writeIndex( QString str)
{
//+ "/index.tpl"
    QDir viewDirectory(str + "/" + this->model.getName().toLower());



    if (!viewDirectory.exists())
        viewDirectory.mkdir(viewDirectory.path());

    str.append("/").append(this->model.getName().toLower()).append("/").append("index.tpl");
    QFile file(str);
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream out(&file);
        out << this->generateIndex();

        // optional, as QFile destructor will already do it:
        file.close();
}

void ZfView::writeForm( QString str)
{
//+ "/index.tpl"
    QDir viewDirectory(str + "/" + this->model.getName().toLower());



    if (!viewDirectory.exists())
        viewDirectory.mkdir(viewDirectory.path());

    str.append("/").append(this->model.getName().toLower()).append("/").append("form.tpl");
    QFile file(str);
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream out(&file);
        out << this->generateForm();

        // optional, as QFile destructor will already do it:
        file.close();
}

void ZfView::writeHistory( QString str)
{
//+ "/index.tpl"
    QDir viewDirectory(str + "/" + this->model.getName().toLower());



    if (!viewDirectory.exists())
        viewDirectory.mkdir(viewDirectory.path());

    str.append("/").append(this->model.getName().toLower()).append("/").append("history.tpl");
    QFile file(str);
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream out(&file);
        out << this->generateHistory();

        // optional, as QFile destructor will already do it:
        file.close();
}
