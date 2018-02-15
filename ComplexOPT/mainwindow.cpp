#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "drawdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    ui->complexView->setWindowParent(this);

    // Initialise toollbar buttons
    newButton = new QAction(QIcon(":/icons/resources/toolbar/new.png"), "", ui->toolBar);



    saveButton = new QAction(QIcon(":/icons/resources/toolbar/save.png"), "", ui->toolBar);

    panButton = new QAction(QIcon(":/icons/resources/toolbar/pan.png"), "", ui->toolBar);
    zoomButton = new QAction(QIcon(":/icons/resources/toolbar/zoom.png"), "", ui->toolBar);
    newtonButton = new QAction(QIcon(":/icons/resources/toolbar/newton.png"), "", ui->toolBar);

    refreshButton = new QAction(QIcon(":/icons/resources/toolbar/refresh.png"), "", ui->toolBar);

    calculatorButton = new QAction(QIcon(":/icons/resources/toolbar/calculator.png"), "", ui->toolBar);

    undoButton = new QAction(QIcon(":/icons/resources/toolbar/undo.png"), "", ui->toolBar);
    redoButton = new QAction(QIcon(":/icons/resources/toolbar/redo.png"), "", ui->toolBar);

    zinButton = new QAction(QIcon(":/icons/resources/toolbar/zoomin.png"), "", ui->toolBar);
    zoutButton = new QAction(QIcon(":/icons/resources/toolbar/zoomout.png"), "", ui->toolBar);

    //Pan and zoom are togglable
    panButton->setCheckable(true);
    zoomButton->setCheckable(true);
    newtonButton->setCheckable(true);

    //Connect toggle slots
    connect(panButton, SIGNAL(toggled(bool)), this, SLOT(togglePan(bool)));
    connect(zoomButton, SIGNAL(toggled(bool)), this, SLOT(toggleZoom(bool)));
    connect(newtonButton, SIGNAL(toggled(bool)), this, SLOT(toggleNewton(bool)));

    //Start application with pan already checked
    panButton->setChecked(true);

    //Allow constant tracking for mouse move
    ui->complexView->setMouseTracking(true);

    //Add buttons with tooltiptext and seperators to toolbar
    ui->toolBar->addAction(newButton);
    ui->toolBar->addSeparator();

    ui->toolBar->addAction(saveButton);
    ui->toolBar->addSeparator();

    ui->toolBar->addAction(panButton);
    ui->toolBar->addAction(zoomButton);
    ui->toolBar->addAction(newtonButton);
    ui->toolBar->addSeparator();

    ui->toolBar->addAction(refreshButton);
    ui->toolBar->addSeparator();

    ui->toolBar->addAction(calculatorButton);
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

    calculatorButton->setToolTip("Calculator dialog");

    undoButton->setToolTip("Undo last action");
    redoButton->setToolTip("Redo last action");

    zinButton->setToolTip("Zoom in");
    zoutButton->setToolTip("Zoom out");

    //Setup new canvas dialog object
    dialog = new DrawDialog(this);

    //Setup calculator
    calculator = new CalculatorDialog(this);

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

    //Allow label to change color
    color->setAutoFillBackground(true);

    //Setup initial color in statusbar
    QPalette p = palette();
    p.setColor(QPalette::Background, Qt::white);
    color->setPalette(p);

    //Bind widgets to statusbar
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

    image = nullptr;


    if (!lib->load()) //If there is an issue loading cooda.dll, inform the user and disable all buttons and functions
    {


        //qDebug() << "Error loading cooda library: " << lib->errorString();

        QMessageBox::critical(this, "Cooda DLL Error", "Error loading cooda.dll library - '" + lib->errorString() + "'", QMessageBox::Ok);

        ui->action_New->setEnabled(false);
        ui->actionRefresh->setEnabled(false);
        ui->actionSave->setEnabled(false);
        ui->action_Redo->setEnabled(false);
        ui->action_Undo->setEnabled(false);

        //Disable all buttons in toolbar
        for (auto& a : ui->toolBar->actions())
            a->setEnabled(false);

    }
    else
    {
        constructor = (DLLConstruct)lib->resolve("entryConstruct");
        initialise = (DLLInitialise)lib->resolve("entryInitialise");
        calculate = (DLLCalculate)lib->resolve("entryCalculate");
        destructor = (DLLDestruct)lib->resolve("entryDestruct");
        trace = (DLLTrace)lib->resolve("entryTrace");
        gradient = (DLLGradient)lib->resolve("entryGradient");
        newton = (DLLNewton)lib->resolve("entryNewtonRaphson");

        //qDebug() << "Construct error: " <<
        constructor(0);


        scene = new QGraphicsScene();
        ui->complexView->setScene(scene);
    }
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

void MainWindow::redraw(Complex minimum, Complex maximum, TokenList list)
{
    /*auto minimum = Complex(min.real(), max.imag());

    auto maximum = Complex(max.real(), min.imag());*/

    if (lib->isLoaded())
    {
        initialise(image->width(), image->height(), list, image->bits());

        calculate(minimum, maximum);

        scene->clear();
        scene->addPixmap(QPixmap::fromImage(*image));
    }
}

void MainWindow::retrace(const QPoint &event)
{
    if (!lib->load())
        return;

    //auto last = dialog->getMax();
    //auto min = dialog->getMin();

    auto first = Complex(dialog->getMin().real(), dialog->getMax().imag());
    auto last = Complex(dialog->getMax().real(), dialog->getMin().imag());

    auto diff = last - first;

    int col;

    double mod, arg;

    Complex ans;

    Complex z = {
            first.real() + diff.real() * (event.x()) / image->width(),
            first.imag() + diff.imag() * (event.y()) / image->height()
        };

    trace(z, dialog->getList(), &ans, &col, &mod, &arg);

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

void MainWindow::find_root(const Complex &c)
{
    auto xn = newton(dialog->getList(), c, 100);

    auto strXn = complex_to_string(xn);

    int col; double mod, arg; Complex fz;

    trace(xn, dialog->getList(), &fz, &col, &mod, &arg);

    auto strFz = complex_to_string(fz);

    auto ret = QMessageBox::question(this, "Root found",
                          "Root found at z = " + strXn + " (f(z) = " + strFz + "). Save to clipboard?",
                          QMessageBox::No | QMessageBox::Yes);

    if (ret == QMessageBox::Yes)
        QApplication::clipboard()->setText(strXn);
}

Complex MainWindow::evaluate(const Complex &z, const TokenList &list)
{
    Complex ans;
    int a;
    double b,c;
    trace(z,list, &ans, &a, &b, &c);
    return ans;
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    ui->complexView->setGeometry(ui->complexView->x(), ui->complexView->y(), event->size().width() - 20, event->size().height() - 20);

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
    else if (action == calculatorButton)
    {
        calculator->show();
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
    else if (action == calculatorButton)
    {

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
