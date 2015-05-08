#include "BackupLayout.h"
#include "ui_BackupLayout.h"

#include <QPainter>
#include <QStandardItemModel>
#include <QStyledItemDelegate>

class GridDelegate : public QStyledItemDelegate
{
public:
    explicit GridDelegate(QObject * parent = 0) : QStyledItemDelegate(parent) { }

    void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const
    {
        painter->save();
        painter->setPen(QColor("#ff0000"));
        painter->drawRect(option.rect);
        painter->restore();

        QStyledItemDelegate::paint(painter, option, index);
    }
};

BackupLayout::BackupLayout(QWidget *parent, const BackupPresenter& backupPresenter) :
    QWidget(parent),
    _presenter(&const_cast<BackupPresenter&>(backupPresenter)),
    ui(new Ui::BackupLayout)
{
    ui->setupUi(this);

    ui->quickWidget->setSource(QUrl("./Widgets/ThumbnailView.qml"));

    //QFileSystemModel* model = new QFileSystemModel();
    //model->setFilter(QDir::Dirs | QDir::NoDotAndDotDot | QDir::AllDirs);
    //model->setSorting(QDir::Name | QDir::IgnoreCase);

    /*ui->treeView->setModel(model);
    ui->treeView->setRootIndex(model->index(QDir::rootPath()));
    ui->treeView->setWindowTitle(QObject::tr("Dir View:")+QDir::homePath());
    */
    QStandardItemModel* model2 = new QStandardItemModel();
    model2->setRowCount(15);
    model2->setColumnCount(2);

    for(int r = 0; r < model2->rowCount(); r++)
    {
        for(int c = 0; c < model2->columnCount(); c++)
        {
            if(c == 0)
            {
                model2->setItem(r, c, new QStandardItem("Property"));
            }
            else
            {
                model2->setItem(r, c, new QStandardItem("Value"));
            }
        }
    }

    model2->setHorizontalHeaderItem(0, new QStandardItem("Property"));
    model2->setHorizontalHeaderItem(1, new QStandardItem("Value"));

    ui->treeView_2->setItemDelegate(new GridDelegate());
    ui->treeView_2->setModel(model2);

    //ui->treeView_3->setRootIndex(model3->index("/media/andi/TOSHIBA EXT"));

    //model3->setFilter(QDir::AllDirs | QDir::NoDotAndDotDot);

    ui->driveList->setModel(_presenter->GetDriveListModel());
    ui->driveList->setCurrentIndex(ui->driveList->model()->index(0,0));

    ui->folderTree->setModel(_presenter->GetFolderTreeModel());
    ui->folderTree->setRootIndex(_presenter->GetFolderTreeModel()->setRootPath("/media/andi/TOSHIBA EXT"));

    //Disable type and size column
    ui->folderTree->setColumnHidden(1, true);
    ui->folderTree->setColumnHidden(2, true);

    //ui->folderTree->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);

    connect(ui->driveList->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), _presenter, SLOT(CurrentDriveChanged(QModelIndex, QModelIndex)));
    connect(ui->driveList->selectionModel(), SIGNAL(dataChanged(QModelIndex,QModelIndex)), SLOT(DriveListChanged()));

    connect(_presenter, SIGNAL(DriveSelectionChanged(QModelIndex)), SLOT(DriveSelectionChanged(QModelIndex)));
}

BackupLayout::~BackupLayout()
{
    delete ui;
}

void BackupLayout::DriveSelectionChanged(QModelIndex driveRoot)
{
    ui->folderTree->setRootIndex(driveRoot);
}

void BackupLayout::DriveListChanged()
{
    int i = 0;
}