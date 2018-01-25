#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->complexView->setWindowParent(this);

    // Initialise toollbar buttons
    newButton = new QAction(QIcon(":/icons/resources/toolbar/new.bmp"), "", ui->toolBar);

    saveButton = new QAction(QIcon(":/icons/resources/toolbar/save.bmp"), "", ui->toolBar);

    panButton = new QAction(QIcon(":/icons/resources/toolbar/pan.bmp"), "", ui->toolBar);
    zoomButton = new QAction(QIcon(":/icons/resources/toolbar/zoom.bmp"), "", ui->toolBar);
    newtonButton = new QAction(QIcon(":/icons/resources/toolbar/newton.png"), "", ui->toolBar);

    refreshButton = new QAction(QIcon(":/icons/resources/toolbar/refresh.bmp"), "", ui->toolBar);

    undoButton = new QAction(QIcon(":/icons/resources/toolbar/undo.bmp"), "", ui->toolBar);
    redoButton = new QAction(QIcon(":/icons/resources/toolbar/redo.bmp"), "", ui->toolBar);

    zinButton = new QAction(QIcon(":/icons/resources/toolbar/zoomin.png"), "", ui->toolBar);
    zoutButton = new QAction(QIcon(":/icons/resources/toolbar/zoomout.png"), "", ui->toolBar);

    //Pan and zoom are togglable
    panButton->setCheckable(true);
    zoomButton->setCheckable(true);
    newtonButton->setCheckable(true);

    //COnnect toggle slots
    connect(panButton, SIGNAL(toggled(bool)), this, SLOT(togglePan(bool)));
    connect(zoomButton, SIGNAL(toggled(bool)), this, SLOT(toggleZoom(bool)));
    connect(newtonButton, SIGNAL(toggled(bool)), this, SLOT(toggleNewton(bool)));

    //Start application with pan already checked
    panButton->setChecked(true);

    //Allow constant tracking for mouse move
    ui->complexView->setMouseTracking(true);

    //Add buttons and seperators to toolbar
    ui->toolBar->addAction(newButton);
    ui->toolBar->addSeparator();

    ui->toolBar->addAction(saveButton);
    ui->toolBar->addSeparator();

    ui->toolBar->addAction(panButton);
    ui->toolBar->addAction(zoomButton);
    ui->toolBar->addSeparator();

    ui->toolBar->addAction(newtonButton);
    ui->toolBar->addSeparator();

    ui->toolBar->addAction(refreshButton);
    ui->toolBar->addSeparator();

    ui->toolBar->addAction(undoButton);
    ui->toolBar->addAction(redoButton);
    ui->toolBar->addSeparator();

    ui->toolBar->addAction(zinButton);
    ui->toolBar->addAction(zoutButton);

    newButton->setToolTip("New");

    saveButton->setToolTip("Save image as...");

    panButton->setToolTip("Pan tool");
    zoomButton->setToolTip("Rectangle zoom tool");
    newtonButton->setToolTip("Zero-finding tool");

    refreshButton->setToolTip("Refresh screen");

    undoButton->setToolTip("Undo last action");
    redoButton->setToolTip("Redo last action");

    zinButton->setToolTip("Zoom in");
    zoutButton->setToolTip("Zoom out");


    //Setup tokens
    auto tokens = new Token[1];
    tokens[0] = { 1, {0, 0} };

    //Setup dialog object
    dialog = new DrawDialog(this);
    dialog->setModal(true);
    dialog->setFormula("z");

    //Setup undo/redo history
    LinearUndo<Canvas>::instance().append({dialog->getMin(), dialog->getMax(), dialog->getFormula()});

    //Setup status bar obnjects
    color = new QLabel("    ");
    inreal = new QLabel("Real: ");
    inimag = new QLabel("Imag: ");
    outreal = new QLabel("Real: ");
    outimag = new QLabel("Imag: ");
    outmod = new QLabel("Mod: ");
    outarg = new QLabel("Arg: ");

    color->setAutoFillBackground(true);

    QPalette p = palette();

    p.setColor(QPalette::Background, Qt::white);

    color->setPalette(p);

    ui->statusBar->addPermanentWidget(color);
    ui->statusBar->addPermanentWidget(inreal);
    ui->statusBar->addPermanentWidget(inimag);
    ui->statusBar->addPermanentWidget(outreal);
    ui->statusBar->addPermanentWidget(outimag);
    ui->statusBar->addPermanentWidget(outmod);
    ui->statusBar->addPermanentWidget(outarg);

    color->setToolTip("Domain color");

    inreal->setToolTip("Domain real part");
    inimag->setToolTip("Domain imaginary part");

    outreal->setToolTip("Codomain real part");
    outimag->setToolTip("Codomain imaginary part");
    outmod->setToolTip("Codomain modulus");
    outarg->setToolTip("Codomain argument");

    lib = new QLibrary("cuRIEMANN.dll");
    if (!lib->load()) //If there is an issue loading cooda.dll, inform the user and disable all buttons and functions
    {


        //qDebug() << "Error loading cooda library: " << lib->errorString();
        QMessageBox* coodaError = new QMessageBox(QMessageBox::Critical, "Cooda DLL Error", "Error loading cooda.dll library - '" + lib->errorString() + "'", QMessageBox::Ok);
        coodaError->show();

        ui->action_New->setEnabled(false);
        ui->actionRefresh->setEnabled(false);
        ui->actionSave->setEnabled(false);
        ui->action_Redo->setEnabled(false);
        ui->action_Undo->setEnabled(false);

        newButton->setEnabled(false);
        saveButton->setEnabled(false);
        panButton->setEnabled(false);
        zoomButton->setEnabled(false);
        newtonButton->setEnabled(false);
        refreshButton->setEnabled(false);
        undoButton->setEnabled(false);
        redoButton->setEnabled(false);
        zinButton->setEnabled(false);
        zoutButton->setEnabled(false);

    }
    else
    {
        constructor = (DLLConstruct)lib->resolve("entryConstruct");
        initialise = (DLLInitialise)lib->resolve("entryInitialise");
        calculate = (DLLCalculate)lib->resolve("entryCalculate");
        destructor = (DLLDestruct)lib->resolve("entryDestruct");
        trace = (DLLTrace)lib->resolve("entryTrace");

        //qDebug() << "Construct error: " <<
        constructor(0);

        image = nullptr;

        scene = new QGraphicsScene();
        ui->complexView->setScene(scene);
    }


    //Work in progress...
    newtonButton->setEnabled(false);


}

void MainWindow::togglePan(bool checked)
{
    if (checked)
    {
        zoomButton->setChecked(false);
        newtonButton->setChecked(false);
    }
}

void MainWindow::toggleZoom(bool checked)
{
    if (checked)
    {
        panButton->setChecked(false);
        newtonButton->setChecked(false);
    }
}

void MainWindow::toggleNewton(bool checked)
{
    if (checked)
    {
        zoomButton->setChecked(false);
        panButton->setChecked(false);
    }
}

void MainWindow::redraw(Complex min, Complex max, TokenList list)
{
    if (lib->isLoaded())
    {


        //qDebug() << "Initialise error:" <<
        initialise(image->width(), image->height(), list, image->bits());

        //qDebug() << "Calculate error: " <<
        calculate(min, max);

        scene->clear();
        scene->addPixmap(QPixmap::fromImage(*image));

    }
}

void MainWindow::retrace(const QPoint &event)
{
    if (!lib->load())
        return;

    auto max = dialog->getMax();

    auto min = dialog->getMin();

    auto diff = max - min;

    int col;

    double mod, arg;

    Complex ans;

    Complex z = {
            min.real() + diff.real() * (event.x()) / image->width(),
            min.imag() + diff.imag() * (event.y()) / image->height()
        };

    trace(z, col, ans, dialog->getList(), mod, arg);

    QPalette p = palette();

    p.setColor(QPalette::Background, QColor::fromRgb(col));

    color->setPalette(p);

    inreal->setText("Real: " + QString::number(z.real()));
    inimag->setText("Imag: " + QString::number(z.imag()));

    outreal->setText("Real: " + QString::number(ans.real()));
    outimag->setText("Imag: " + QString::number(ans.imag()));

    outmod->setText("Mod: " + QString::number(mod));
    outarg->setText("Arg: " + QString::number(arg));

}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    ui->complexView->setGeometry(ui->complexView->x(), ui->complexView->y(), ui->centralWidget->width() - 20, ui->centralWidget->height() - 20);

    if (!lib->load())
        return;

    delete image;
    image = new QImage(ui->complexView->width() - 2, ui->complexView->height() - 2, QImage::Format_RGB32);
    scene->setSceneRect(0,0, ui->complexView->width() - 2, ui->complexView->height() - 2);

    buttonRefresh();
}

void MainWindow::toolbarTriggered(QAction *action)
{
    auto tag = action->text();

    if (action == newButton)
    {
        if (dialog->exec())
            buttonNew();
    }
    else if (action == saveButton)
    {
        buttonSave();
    }
    else if (action == panButton)
    {
        if (!panButton->isChecked())
        {
            panButton->setChecked(true);
            zoomButton->setChecked(false);
        }
    }
    else if (action == zoomButton)
    {
        if (!zoomButton->isChecked())
        {
            zoomButton->setChecked(true);
            panButton->setChecked(false);
        }
    }
    else if (action == refreshButton)
    {
        buttonRefresh();
    }
    else if (action == undoButton)
    {
        buttonUndo();
    }
    else if (action == redoButton)
    {
        buttonRedo();
    }
    else if (action == zinButton)
    {
        centerZoom(-1.0/3.0);
    }
    else if (action == zoutButton)
    {
        centerZoom(1.0);
    }
}

void MainWindow::menuTriggered(QAction *action)
{
    if (action->text() == "&New")
    {
        if (dialog->exec())
            buttonNew();
    }
    else if (action->text() == "&Save Image")
    {
        buttonSave();
    }
    else if (action->text() == "Refresh")
    {
        buttonRefresh();
    }
    else if (action->text() == "&Exit")
    {

    }
    else if (action->text() == "&Undo")
    {
        buttonUndo();
    }
    else if (action->text() == "&Redo")
    {
        buttonRedo();
    }
}

void MainWindow:: buttonNew()
{
    LinearUndo<Canvas>::instance().append({dialog->getMin(), dialog->getMax(), dialog->getFormula()});





    redraw(dialog->getMin(), dialog->getMax(), dialog->getList());
}

//(((((z^2 + z)^2 + z)^2 + z)^2 + z) ^ 2 + z)^2 + z
void MainWindow::buttonSave()
{
    auto filename = QFileDialog::getSaveFileName(this,
                                                 tr("Save Image"), "untitled.png", tr("PNG file (*.png);;Jpg file (*.jpg);;Bitmap file (*.bmp)"));
    image->save(filename);
}

void MainWindow::buttonRefresh()
{
    redraw(dialog->getMin(), dialog->getMax(), dialog->getList());
}

void MainWindow::buttonUndo()
{
    //Get previous canvas in history list, provided we are not at the beginning of the list
    auto current = LinearUndo<Canvas>::instance().undo();

    dialog->setMin(current->m_Minimum);
    dialog->setMax(current->m_Maximum);
    dialog->setFormula(current->m_Formula);

    redraw(*current);

}

void MainWindow::centerZoom(double factor)
{
    auto min = dialog->getMin();
    auto max = dialog->getMax();

    auto diff = max - min;

    dialog->setMin(min - diff * factor);
    dialog->setMax(max + diff * factor);


    LinearUndo<Canvas>::instance().append({dialog->getMin(), dialog->getMax(), dialog->getFormula()});

    redraw(dialog->getMin(), dialog->getMax(), dialog->getList());
}

void MainWindow::buttonRedo()
{
    auto current = LinearUndo<Canvas>::instance().redo();

    dialog->setMin(current->m_Minimum);
    dialog->setMax(current->m_Maximum);
    dialog->setFormula(current->m_Formula);

    redraw(*current);
}

MainWindow::~MainWindow()
{
    if (lib->isLoaded())
        //qDebug() << "Destruct error: " <<
        destructor();

    delete image;

    delete ui;
}
