#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QItemDelegate>
#include <QStandardItemModel>
#include <QPainter>
#include <QFileDialog>
#include <QTimer>
#include <QGridLayout>
#include <QStorageInfo>
#include <QDebug>

#include "API/IDataProvider.h"

#include "MediaExplorerView.h"
#include "PlaybackSlider.h"
#include "PreviewPane.h"
#include "TestPluginA.h"

#include "Console.h"
#include "ProgressDialog.h"

#include "Layouts/BackupLayout.h"
#include "Layouts/ClipProcessorLayout.h"
#include "Presenter/BackupPresenter.h"

//#include <dirent.h>

/*struct PreviewImageStruct
{
    int width;
    int height;

    unsigned int dataSize;
    unsigned char* dataPtr;
};

std::vector<std::string> files;
std::vector<PreviewImageStruct*> memoryImages;*/

//QGraphicsScene* scene;

MainWindow::MainWindow(OCContext* context, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QStorageInfo storage(qApp->applicationDirPath());
    qDebug() << storage.rootPath();
    if (storage.isReadOnly())
        qDebug() << "isReadOnly:" << storage.isReadOnly();

    qDebug() << "name:" << storage.name();
    qDebug() << "fileSystemType:" << storage.fileSystemType();
    qDebug() << "size:" << storage.bytesTotal()/1000/1000 << "MB";
    qDebug() << "availableSize:" << storage.bytesAvailable()/1000/1000 << "MB";



    QList<QStorageInfo> drives = QStorageInfo::mountedVolumes();

    for(auto& drive : drives)
    {
       qDebug() << drive.rootPath();
       if (drive.isReadOnly())
           qDebug() << "isReadOnly:" << drive.isReadOnly();

       qDebug() << "name:" << drive.name();
       qDebug() << "fileSystemType:" << drive.fileSystemType();
       qDebug() << "size:" << drive.bytesTotal()/1000/1000 << "MB";
       qDebug() << "availableSize:" << drive.bytesAvailable()/1000/1000 << "MB";
    }
    _context = context;

    MediaExplorerPresenter* mediaExplorerPresenter = new MediaExplorerPresenter(_context);
    PlaybackPresenter* playbackPresenter = new PlaybackPresenter(_context);

    //Add preview pane
    //ui->gridLayout_3->addWidget(new PreviewPane(playbackPresenter));

    //Set Media Explorer widget
    //ui->dockWidget_3->setWidget(new MediaExplorerView(mediaExplorerPresenter));

    //QGridLayout* layout = new QGridLayout();
    //layout->addWidget(new PlaybackSlider(playbackPresenter)), ui->widget->setLayout(layout);

    stdout = freopen("output_file", "w", stdout);

    QFile f;
    f.open(stderr, QIODevice::ReadOnly);
    QByteArray output = f.readAll();
    QString out(output);

    /*ui->label_2->setText(out);
    ui->textBrowser->setText(out);

    ui->textBrowser->append("TEST\n");
    ui->textBrowser->append("12345\n");
*/
    BackupPresenter* backupPresenter = new BackupPresenter(context);
    ui->stackedWidget->addWidget(new BackupLayout(this, *backupPresenter));

    ui->stackedWidget->addWidget(new ClipProcessorLayout());

    //ui->buttonGroup->addButton(new QPushButton("Test", this));

    connect(ui->buttonGroup, SIGNAL(buttonClicked(int)), SLOT(on_pushButton_7_clicked(int)));

    /*QMenu* importMenu = new QMenu();
    QAction* testAction = new QAction("test menu item", this);
    connect(testAction,SIGNAL(triggered()), SLOT(SelectImportFolder()));
    importMenu->addAction(testAction);*/
}

/*int currentImageIndex = 0;
QImage* previewImage;
bool rrr = false;

void MainWindow::PlayImages()
{
    PreviewImageStruct* im = memoryImages.at(currentImageIndex);

    if(!rrr)
    {
        previewImage = new QImage((int)im->width, (int)im->height, QImage::Format_RGB888);
        rrr = true;
    }

    //imageProcessor->dcraw_ppm_tiff_writer("test456.ppm");

    for (unsigned int y = 0; y < previewImage->height(); y++)
    {
        memcpy(previewImage->scanLine(y), &im->dataPtr[y * previewImage->bytesPerLine()], previewImage->bytesPerLine());
    }

    it->setPixmap(QPixmap::fromImage(*previewImage));
    //scene->clear();
    //scene->addPixmap(QPixmap::fromImage(*previewImage));
    //scene->addItem(it);

    //scene->invalidate();

    currentImageIndex++;
    if(currentImageIndex == files.size())
    {
        currentImageIndex = 0;
    }

    ui->frameCount->setText(QString::number(currentImageIndex));

    //imageProcessor->recycle();
}*/

MainWindow::~MainWindow()
{
    //imageProcessor->recycle();
    delete ui;
}

//void MainWindow::resizeEvent(QResizeEvent *)
//{
//ui->previewArea->fitInView(it, Qt::KeepAspectRatio);
//}

/*void MainWindow::SelectImportFolder()
{
    //QFileDialog* dialog = new QFileDialog(this);
    //dialog->setFileMode(QFileDialog::Directory);
    //dialog->setOption(QFileDialog::ShowDirsOnly);
    //dialog->show();
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    "/home",
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    int i = 0;
}*/
void MainWindow::on_aboutButton_clicked()
{
    QMessageBox msgBox;
    msgBox.setText("About");
    msgBox.exec();
}

void MainWindow::on_pushButton_clicked()
{
    ProgressDialog* progressDialog = new ProgressDialog(nullptr, new DriveTransfer(nullptr, ".", "./Test"));
    progressDialog->show();
    //Console* console = new Console(this);
    //console->show();
}

void MainWindow::on_pushButton_7_clicked(int id)
{
    ui->stackedWidget->setCurrentIndex(id);
}
