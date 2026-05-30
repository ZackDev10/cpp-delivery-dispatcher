#include "gui/MainWindow.h"

#include <QAbstractItemView>
#include <QComboBox>
#include <QDateTime>
#include <QFormLayout>
#include <QGridLayout>
#include <QGraphicsDropShadowEffect>
#include <QGraphicsOpacityEffect>
#include <QGroupBox>
#include <QHeaderView>
#include <QIcon>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QListWidget>
#include <QPainter>
#include <QProgressBar>
#include <QPushButton>
#include <QPropertyAnimation>
#include <QSaveFile>
#include <QStatusBar>
#include <QStyle>
#include <QTabWidget>
#include <QTimer>
#include <QTableWidget>
#include <QVBoxLayout>

#include <sstream>
#include <QCloseEvent>
#include <QCoreApplication>
#include "include/Transporteur.h"
#include <QDir>

#include "include/Avion.h"
#include "include/Camion.h"
#include "include/Drone.h"
#include "include/Velo.h"

namespace
{
    TypeColis typeFromInt(int value)
    {
        switch (value)
        {
        case 0: return TypeColis::STANDARD;
        case 1: return TypeColis::FRAGILE;
        case 2: return TypeColis::PERISSABLE;
        case 3: return TypeColis::DANGEREUX;
        default: return TypeColis::STANDARD;
        }
    }

    StatutColis statutFromInt(int value)
    {
        switch (value)
        {
        case 0: return StatutColis::EN_ATTENTE;
        case 1: return StatutColis::EN_TRANSIT;
        case 2: return StatutColis::LIVRE;
        case 3: return StatutColis::ANNULE;
        default: return StatutColis::EN_ATTENTE;
        }
    }

    std::string toString(TypeColis type)
    {
        switch (type)
        {
        case TypeColis::STANDARD:
            return "Standard";
        case TypeColis::FRAGILE:
            return "Fragile";
        case TypeColis::PERISSABLE:
            return "Périssable";
        case TypeColis::DANGEREUX:
            return "Dangereux";
        default:
            return "-";
        }
    }

    std::string toString(StatutColis statut)
    {
        switch (statut)
        {
        case StatutColis::EN_ATTENTE:
            return "En attente";
        case StatutColis::EN_TRANSIT:
            return "En transit";
        case StatutColis::LIVRE:
            return "Livré";
        case StatutColis::ANNULE:
            return "Annulé";
        default:
            return "-";
        }
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    systeme_.ajouterTransporteur(new Velo("Velo Express"));
    systeme_.ajouterTransporteur(new Drone("Drone Alpha"));
    systeme_.ajouterTransporteur(new Camion("Camion Nord"));
    systeme_.ajouterTransporteur(new Avion("Air Rapide"));
    rapporteur_.logEvenement("Systeme GUI demarre avec 4 vehicules.");

    buildUi();
    loadColisFromJson();
    refreshStats();
    refreshTable();
}

void MainWindow::buildUi()
{
    setWindowTitle("Simulateur Logistique - GUI");
    QIcon appIcon(":/assets/logo.png");
    if (appIcon.isNull())
    {
        appIcon = style()->standardIcon(QStyle::SP_DesktopIcon);
    }
    setWindowIcon(appIcon);
    resize(1280, 760);

    setStyleSheet(
        "QWidget { font-family: 'Segoe UI'; font-size: 10.5pt; color: #f8fafc; }"
        "QMainWindow { background: #0b0f14; }"
        "QWidget#centralSurface { background: #0b0f14; }"
        "QTabWidget { background: #0b0f14; }"
        "QTabWidget::pane { background: #0b0f14; border: none; }"
        "QWidget#dashboardPage, QWidget#operationsPage, QWidget#stockPage, QWidget#reportsPage { background: #0b0f14; }"
        "QGroupBox { background: #0f172a; border: 1px solid #1f2937; border-radius: 12px; margin-top: 14px; }"
        "QGroupBox::title { subcontrol-origin: margin; left: 14px; padding: 0 6px; color: #f8fafc; font-weight: 600; }"
        "QLineEdit, QComboBox { background: #111827; border: 1px solid #334155; border-radius: 8px; padding: 6px 8px; color: #f8fafc; }"
        "QLineEdit:focus, QComboBox:focus { border: 1px solid #60a5fa; }"
        "QPushButton { background: #111827; color: #f8fafc; border: 1px solid #334155; border-radius: 8px; padding: 8px 14px; font-weight: 600; }"
        "QPushButton:hover { background: #1f2937; }"
        "QPushButton:pressed { background: #0f172a; }"
        "QTableWidget { background: #0f172a; border: 1px solid #1f2937; border-radius: 12px; gridline-color: #1f2937; color: #f8fafc; }"
        "QHeaderView::section { background: #111827; padding: 6px; border: none; font-weight: 600; color: #f8fafc; }"
        "QProgressBar { background: #111827; border-radius: 6px; height: 14px; text-align: left; padding-left: 6px; color: #f8fafc; }"
        "QProgressBar::chunk { background: #334155; border-radius: 6px; }"
        "QStatusBar { background: #0f172a; border-top: 1px solid #1f2937; color: #f8fafc; }"
    );

    auto *central = new QWidget(this);
    central->setObjectName("centralSurface");
    auto *rootLayout = new QHBoxLayout(central);
    rootLayout->setContentsMargins(16, 16, 16, 16);
    rootLayout->setSpacing(16);

    sidebar_ = new QListWidget();
    sidebar_->addItem(new QListWidgetItem(style()->standardIcon(QStyle::SP_DesktopIcon), "Dashboard"));
    sidebar_->addItem(new QListWidgetItem(style()->standardIcon(QStyle::SP_FileDialogNewFolder), "Opérations"));
    sidebar_->addItem(new QListWidgetItem(style()->standardIcon(QStyle::SP_FileDialogContentsView), "Stock"));
    sidebar_->addItem(new QListWidgetItem(style()->standardIcon(QStyle::SP_DialogSaveButton), "Rapports"));
    sidebar_->setFixedWidth(200);
    sidebar_->setCurrentRow(0);
    sidebar_->setStyleSheet("QListWidget { background:#0f172a; color:#f8fafc; border: 1px solid #1f2937; border-radius: 12px; padding: 6px; outline: 0; }"
                            "QListWidget::item { padding: 12px 10px; border-radius: 10px; border: 1px solid transparent; }"
                            "QListWidget::item:selected { background:#1f2937; color:#f8fafc; border: 1px solid transparent; }"
                            "QListWidget::item:selected:focus { outline: 0; border: 1px solid transparent; }"
                            "QListWidget::item:focus { outline: 0; border: 1px solid transparent; }"
                            "QListWidget::item:hover { background:#111827; }");

    tabs_ = new QTabWidget();
    tabs_->tabBar()->hide();

    rootLayout->addWidget(sidebar_);
    rootLayout->addWidget(tabs_, 1);

    auto *dashboardPage = new QWidget();
    dashboardPage->setObjectName("dashboardPage");
    auto *dashboardLayout = new QVBoxLayout(dashboardPage);
    dashboardLayout->setSpacing(16);

    auto *titleRow = new QHBoxLayout();
    logoLabel_ = new QLabel();
    QPixmap logo(":/assets/logo.png");
    if (logo.isNull())
    {
        logo = QPixmap(56, 56);
        logo.fill(Qt::transparent);
        QPainter painter(&logo);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setBrush(QColor("#111827"));
        painter.setPen(Qt::NoPen);
        painter.drawRoundedRect(0, 0, 56, 56, 12, 12);
        painter.setPen(Qt::white);
        painter.setFont(QFont("Segoe UI", 18, QFont::Bold));
        painter.drawText(logo.rect(), Qt::AlignCenter, "SL");
    }
    logoLabel_->setPixmap(logo.scaled(56, 56, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    auto *title = new QLabel("<h2>Simulateur Logistique — Tableau de bord</h2>");
    auto *subtitle = new QLabel("Gestion professionnelle des colis et transporteurs");
    subtitle->setStyleSheet("color: #cbd5f5;");

    auto *titleText = new QVBoxLayout();
    titleText->addWidget(title);
    titleText->addWidget(subtitle);
    titleRow->addWidget(logoLabel_);
    titleRow->addLayout(titleText);
    titleRow->addStretch();
    dashboardLayout->addLayout(titleRow);

    auto *cardsLayout = new QHBoxLayout();
    cardsLayout->setSpacing(12);
    cardsLayout->addWidget(createStatCard("Total", totalLabel_));
    cardsLayout->addWidget(createStatCard("En attente", attenteLabel_));
    cardsLayout->addWidget(createStatCard("En transit", transitLabel_));
    cardsLayout->addWidget(createStatCard("Livré", livreLabel_));
    cardsLayout->addWidget(createStatCard("Annulé", annuleLabel_));
    dashboardLayout->addLayout(cardsLayout);

    auto *chartBox = new QGroupBox("Répartition des statuts");
    auto *chartLayout = new QVBoxLayout(chartBox);
    attenteBar_ = new QProgressBar();
    transitBar_ = new QProgressBar();
    livreBar_ = new QProgressBar();
    annuleBar_ = new QProgressBar();
    attenteBar_->setFormat("En attente: %v");
    transitBar_->setFormat("En transit: %v");
    livreBar_->setFormat("Livré: %v");
    annuleBar_->setFormat("Annulé: %v");
    chartLayout->addWidget(attenteBar_);
    chartLayout->addWidget(transitBar_);
    chartLayout->addWidget(livreBar_);
    chartLayout->addWidget(annuleBar_);
    dashboardLayout->addWidget(chartBox);

    auto *dailyBox = new QGroupBox("Colis enregistrés aujourd'hui");
    auto *dailyLayout = new QVBoxLayout(dailyBox);
    dailyTable_ = new QTableWidget(0, 7);
    dailyTable_->setHorizontalHeaderLabels({"ID", "Type", "Statut", "Poids", "Distance", "Volume", "Destination"});
    dailyTable_->horizontalHeader()->setStretchLastSection(true);
    dailyTable_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    dailyTable_->setSelectionBehavior(QAbstractItemView::SelectRows);
    dailyTable_->setAlternatingRowColors(true);
    dailyLayout->addWidget(dailyTable_);
    dashboardLayout->addWidget(dailyBox);

    dashboardLayout->addStretch();

    auto *operationsPage = new QWidget();
    operationsPage->setObjectName("operationsPage");
    auto *operationsLayout = new QVBoxLayout(operationsPage);
    operationsLayout->setSpacing(16);

    auto *createBox = new QGroupBox("Créer un colis");
    auto *createLayout = new QFormLayout(createBox);
    idInput_ = new QLineEdit();
    poidsInput_ = new QLineEdit();
    distanceInput_ = new QLineEdit();
    longueurInput_ = new QLineEdit();
    largeurInput_ = new QLineEdit();
    hauteurInput_ = new QLineEdit();
    destinationInput_ = new QLineEdit();
    typeCombo_ = new QComboBox();
    typeCombo_->addItems({"Standard", "Fragile", "Périssable", "Dangereux"});

    createLayout->addRow("ID", idInput_);
    createLayout->addRow("Poids (kg)", poidsInput_);
    createLayout->addRow("Distance (km)", distanceInput_);
    createLayout->addRow("Longueur (cm)", longueurInput_);
    createLayout->addRow("Largeur (cm)", largeurInput_);
    createLayout->addRow("Hauteur (cm)", hauteurInput_);
    createLayout->addRow("Type", typeCombo_);
    createLayout->addRow("Destination", destinationInput_);

    auto *createButton = new QPushButton("Créer le colis");
    createButton->setIcon(style()->standardIcon(QStyle::SP_DialogApplyButton));
    connect(createButton, &QPushButton::clicked, this, &MainWindow::onCreateColis);
    createLayout->addRow(createButton);

    auto *statusBox = new QGroupBox("Mise à jour du statut");
    auto *statusLayout = new QFormLayout(statusBox);
    updateIdInput_ = new QLineEdit();
    statutCombo_ = new QComboBox();
    statutCombo_->addItems({"En attente", "En transit", "Livré", "Annulé"});
    statusLayout->addRow("ID Colis", updateIdInput_);
    statusLayout->addRow("Statut", statutCombo_);
    auto *statusButton = new QPushButton("Mettre à jour");
    statusButton->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));
    connect(statusButton, &QPushButton::clicked, this, &MainWindow::onUpdateStatut);
    statusLayout->addRow(statusButton);

    auto *assignBox = new QGroupBox("Assigner transporteur");
    auto *assignLayout = new QFormLayout(assignBox);
    assignIdInput_ = new QLineEdit();
    assignLayout->addRow("ID Colis", assignIdInput_);
    auto *assignButton = new QPushButton("Assigner");
    assignButton->setIcon(style()->standardIcon(QStyle::SP_CommandLink));
    connect(assignButton, &QPushButton::clicked, this, &MainWindow::onAssignTransporteur);
    assignLayout->addRow(assignButton);

    operationsLayout->addWidget(createBox);
    operationsLayout->addWidget(statusBox);
    operationsLayout->addWidget(assignBox);
    operationsLayout->addStretch();

    auto *stockPage = new QWidget();
    stockPage->setObjectName("stockPage");
    auto *stockLayout = new QVBoxLayout(stockPage);
    stockLayout->setSpacing(16);

    stockTable_ = new QTableWidget(0, 9);
    stockTable_->setHorizontalHeaderLabels({"ID", "Type", "Statut", "Poids", "Distance", "Volume", "Transporteur", "Prix", "Vitesse"});
    stockTable_->horizontalHeader()->setStretchLastSection(true);
    stockTable_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    stockTable_->setSelectionBehavior(QAbstractItemView::SelectRows);
    stockTable_->setAlternatingRowColors(true);
    stockLayout->addWidget(stockTable_);

    auto *reportsPage = new QWidget();
    reportsPage->setObjectName("reportsPage");
    auto *reportsLayout = new QVBoxLayout(reportsPage);
    reportsLayout->setSpacing(16);
    auto *exportButton = new QPushButton("Exporter les logs");
    exportButton->setIcon(style()->standardIcon(QStyle::SP_DialogSaveButton));
    connect(exportButton, &QPushButton::clicked, this, &MainWindow::onExportLogs);
    reportsLayout->addWidget(new QLabel("Export des journaux d'activité"));
    reportsLayout->addWidget(exportButton);
    reportsLayout->addStretch();

    tabs_->addTab(dashboardPage, "Dashboard");
    tabs_->addTab(operationsPage, "Opérations");
    tabs_->addTab(stockPage, "Stock");
    tabs_->addTab(reportsPage, "Rapports");

    connect(sidebar_, &QListWidget::currentRowChanged, tabs_, &QTabWidget::setCurrentIndex);

    setCentralWidget(central);
    statusBar()->showMessage("Prêt à gérer vos colis", 4000);
}

QFrame *MainWindow::createStatCard(const QString &title, QLabel *&valueLabel)
{
    auto *card = new QFrame();
    card->setFrameShape(QFrame::StyledPanel);
    card->setStyleSheet("QFrame { background: #111827; border: 1px solid #1f2937; border-radius: 12px; }");
    card->setMinimumHeight(90);
    auto *shadow = new QGraphicsDropShadowEffect(card);
    shadow->setBlurRadius(14);
    shadow->setOffset(0, 4);
    shadow->setColor(QColor(15, 23, 42, 40));
    card->setGraphicsEffect(shadow);
    auto *layout = new QVBoxLayout(card);
    auto *titleLabel = new QLabel(title);
    titleLabel->setStyleSheet("color:#f8fafc; font-weight: 700;");
    valueLabel = new QLabel("0");
    valueLabel->setStyleSheet("font-size: 22px; font-weight: 800; color:#f8fafc;");
    layout->addWidget(titleLabel);
    layout->addWidget(valueLabel);
    return card;
}

void MainWindow::refreshStats()
{
    const auto &colis = systeme_.getColis();
    int attente = 0, transit = 0, livre = 0, annule = 0;
    for (const auto &c : colis)
    {
        switch (c.getStatut())
        {
        case StatutColis::EN_ATTENTE:
            ++attente;
            break;
        case StatutColis::EN_TRANSIT:
            ++transit;
            break;
        case StatutColis::LIVRE:
            ++livre;
            break;
        case StatutColis::ANNULE:
            ++annule;
            break;
        }
    }

    totalLabel_->setText(QString::number(static_cast<int>(colis.size())));
    attenteLabel_->setText(QString::number(attente));
    transitLabel_->setText(QString::number(transit));
    livreLabel_->setText(QString::number(livre));
    annuleLabel_->setText(QString::number(annule));

    int total = std::max(1, static_cast<int>(colis.size()));
    attenteBar_->setRange(0, total);
    transitBar_->setRange(0, total);
    livreBar_->setRange(0, total);
    annuleBar_->setRange(0, total);
    attenteBar_->setValue(attente);
    transitBar_->setValue(transit);
    livreBar_->setValue(livre);
    annuleBar_->setValue(annule);

    refreshDailyTable();
}

void MainWindow::refreshTable()
{
    const auto &colis = systeme_.getColis();
    stockTable_->setRowCount(static_cast<int>(colis.size()));
    int row = 0;
    for (const auto &c : colis)
    {
        stockTable_->setItem(row, 0, new QTableWidgetItem(QString::number(c.getId())));
        stockTable_->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(toString(c.getType()))));
        stockTable_->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(toString(c.getStatut()))));
        stockTable_->setItem(row, 3, new QTableWidgetItem(QString::number(c.getPoids(), 'f', 2)));
        stockTable_->setItem(row, 4, new QTableWidgetItem(QString::number(c.getDistance(), 'f', 1)));
        stockTable_->setItem(row, 5, new QTableWidgetItem(QString::number(c.getVolume(), 'f', 2)));
        if (assignments_.contains(c.getId()))
        {
            const auto info = assignments_.value(c.getId());
            stockTable_->setItem(row, 6, new QTableWidgetItem(info.transporteur));
            stockTable_->setItem(row, 7, new QTableWidgetItem(QString::number(info.prix, 'f', 2)));
            stockTable_->setItem(row, 8, new QTableWidgetItem(QString::number(info.vitesse, 'f', 1)));
        }
        else
        {
            stockTable_->setItem(row, 6, new QTableWidgetItem("-"));
            stockTable_->setItem(row, 7, new QTableWidgetItem("-"));
            stockTable_->setItem(row, 8, new QTableWidgetItem("-"));
        }
        ++row;
    }
}

void MainWindow::refreshDailyTable()
{
    const auto &colis = systeme_.getColis();
    const QDate today = QDate::currentDate();

    int rows = 0;
    for (const auto &c : colis)
    {
        if (createdDates_.value(c.getId()) == today)
        {
            ++rows;
        }
    }

    dailyTable_->setRowCount(rows);
    int row = 0;
    for (const auto &c : colis)
    {
        if (createdDates_.value(c.getId()) != today)
        {
            continue;
        }
        dailyTable_->setItem(row, 0, new QTableWidgetItem(QString::number(c.getId())));
        dailyTable_->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(toString(c.getType()))));
        dailyTable_->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(toString(c.getStatut()))));
        dailyTable_->setItem(row, 3, new QTableWidgetItem(QString::number(c.getPoids(), 'f', 2)));
        dailyTable_->setItem(row, 4, new QTableWidgetItem(QString::number(c.getDistance(), 'f', 1)));
        dailyTable_->setItem(row, 5, new QTableWidgetItem(QString::number(c.getVolume(), 'f', 2)));
        dailyTable_->setItem(row, 6, new QTableWidgetItem(QString::fromStdString(c.getDestination())));
        ++row;
    }
}

void MainWindow::setStatusMessage(const QString &message, bool isError)
{
    statusBar()->setStyleSheet(isError ? "color: #b91c1c;" : "color: #10b981;");
    statusBar()->showMessage(message, 6000);
}

void MainWindow::loadColisFromJson()
{
    const QString dataDir = QDir::current().filePath("data");
    const QString filePath = QDir(dataDir).filePath("colis.json");
    QFile file(filePath);
    if (!file.exists())
        return;
    if (!file.open(QIODevice::ReadOnly))
        return;

    const auto document = QJsonDocument::fromJson(file.readAll());
    file.close();
    if (!document.isArray())
        return;

    for (const auto &value : document.array())
    {
        if (!value.isObject())
            continue;
        const auto obj = value.toObject();
        const int id = obj.value("id").toInt();
        const float poids = static_cast<float>(obj.value("poids").toDouble());
        const float distance = static_cast<float>(obj.value("distance").toDouble());
        const float longueur = static_cast<float>(obj.value("longueur").toDouble());
        const float largeur = static_cast<float>(obj.value("largeur").toDouble());
        const float hauteur = static_cast<float>(obj.value("hauteur").toDouble());
        const int typeValue = obj.value("type").toInt();
        const int statutValue = obj.value("statut").toInt();
    const std::string destination = obj.value("destination").toString().toStdString();
    const QDate createdDate = QDate::fromString(obj.value("date").toString(), Qt::ISODate);

        try
        {
            systeme_.ajouterColis(Colis(id, poids, {longueur, largeur, hauteur}, typeFromInt(typeValue), distance, destination, statutFromInt(statutValue)));
            createdDates_.insert(id, createdDate.isValid() ? createdDate : QDate::currentDate());
        }
        catch (...) {
        }
    }
}

void MainWindow::saveColisToJson() const
{
    const QString dataDir = QDir::current().filePath("data");
    QDir().mkpath(dataDir);
    const QString filePath = QDir(dataDir).filePath("colis.json");

    QJsonArray array;
    for (const auto &c : systeme_.getColis())
    {
        const Dimensions dim = c.getDimensions();
        QJsonObject obj;
        obj.insert("id", c.getId());
        obj.insert("poids", c.getPoids());
        obj.insert("distance", c.getDistance());
        obj.insert("longueur", dim.longueur);
        obj.insert("largeur", dim.largeur);
        obj.insert("hauteur", dim.hauteur);
        obj.insert("type", static_cast<int>(c.getType()));
        obj.insert("statut", static_cast<int>(c.getStatut()));
        obj.insert("destination", QString::fromStdString(c.getDestination()));
        const QDate createdDate = createdDates_.value(c.getId(), QDate::currentDate());
        obj.insert("date", createdDate.toString(Qt::ISODate));
        array.append(obj);
    }

    QSaveFile file(filePath);
    if (!file.open(QIODevice::WriteOnly))
        return;
    file.write(QJsonDocument(array).toJson(QJsonDocument::Indented));
    file.commit();
}

void MainWindow::onCreateColis()
{
    bool ok = false;
    int id = idInput_->text().toInt(&ok);
    if (!ok)
        return setStatusMessage("ID invalide", true);

    float poids = poidsInput_->text().toFloat(&ok);
    if (!ok)
        return setStatusMessage("Poids invalide", true);

    float distance = distanceInput_->text().toFloat(&ok);
    if (!ok)
        return setStatusMessage("Distance invalide", true);

    float longueur = longueurInput_->text().toFloat(&ok);
    if (!ok)
        return setStatusMessage("Longueur invalide", true);

    float largeur = largeurInput_->text().toFloat(&ok);
    if (!ok)
        return setStatusMessage("Largeur invalide", true);

    float hauteur = hauteurInput_->text().toFloat(&ok);
    if (!ok)
        return setStatusMessage("Hauteur invalide", true);

    const std::string destination = destinationInput_->text().toStdString();
    const int typeIndex = typeCombo_->currentIndex();

    try
    {
    systeme_.ajouterColis(Colis(id, poids, {longueur, largeur, hauteur}, static_cast<TypeColis>(typeIndex), distance, destination));
    createdDates_.insert(id, QDate::currentDate());
        rapporteur_.logEvenement("Colis ID " + std::to_string(id) + " cree via GUI.");
        refreshStats();
        refreshTable();
        setStatusMessage("Colis créé avec succès");
        QMessageBox::information(this, "Succès", "Colis créé avec succès.");
        saveColisToJson();
        idInput_->clear();
        poidsInput_->clear();
        distanceInput_->clear();
        longueurInput_->clear();
        largeurInput_->clear();
        hauteurInput_->clear();
        destinationInput_->clear();
        typeCombo_->setCurrentIndex(0);
        idInput_->setFocus();
    }
    catch (const std::exception &e)
    {
        setStatusMessage(QString::fromStdString(e.what()), true);
    }
}

void MainWindow::onUpdateStatut()
{
    bool ok = false;
    int id = updateIdInput_->text().toInt(&ok);
    if (!ok)
        return setStatusMessage("ID invalide", true);

    int statutIndex = statutCombo_->currentIndex();
    try
    {
        systeme_.mettreAJourStatut(id, static_cast<StatutColis>(statutIndex));
        refreshStats();
        refreshTable();
        setStatusMessage("Statut mis à jour");
        QMessageBox::information(this, "Succès", "Statut mis à jour avec succès.");
        updateIdInput_->clear();
        statutCombo_->setCurrentIndex(0);
        updateIdInput_->setFocus();
        saveColisToJson();
    }
    catch (const std::exception &e)
    {
        setStatusMessage(QString::fromStdString(e.what()), true);
    }
}

void MainWindow::onAssignTransporteur()
{
    bool ok = false;
    int id = assignIdInput_->text().toInt(&ok);
    if (!ok)
        return setStatusMessage("ID invalide", true);

    const auto &colis = systeme_.getColis();
    for (const auto &c : colis)
    {
        if (c.getId() == id)
        {
            Transporteur *transporteur = systeme_.assignerTransporteur(c);
            if (transporteur)
            {
                AssignmentInfo info;
                info.transporteur = QString::fromStdString(transporteur->getNom());
                info.prix = transporteur->computeCost(c);
                info.vitesse = transporteur->getVitesse();
                assignments_.insert(id, info);
                refreshTable();
                setStatusMessage("Transporteur assigné");
                QMessageBox::information(this, "Succès", "Transporteur assigné avec succès.");
                assignIdInput_->clear();
                assignIdInput_->setFocus();
                saveColisToJson();
            }
            else
            {
                setStatusMessage("Aucun transporteur disponible", true);
                QMessageBox::warning(this, "Info", "Aucun transporteur disponible pour ce colis.");
            }
            return;
        }
    }

    setStatusMessage("Colis introuvable", true);
}

void MainWindow::onExportLogs()
{
    const QString defaultName = QDir::current().filePath("rapport_final.txt");
    const QString filePath = QFileDialog::getSaveFileName(this, "Exporter les logs", defaultName, "Fichiers texte (*.txt)");
    if (filePath.isEmpty())
        return;

    std::vector<std::string> assignmentLines;
    assignmentLines.reserve(assignments_.size());
    for (auto it = assignments_.cbegin(); it != assignments_.cend(); ++it)
    {
        const auto info = it.value();
        std::ostringstream line;
        line << "Colis #" << it.key() << " -> " << info.transporteur.toStdString()
             << " | " << std::fixed << std::setprecision(2) << info.prix << " MAD"
             << " | " << std::fixed << std::setprecision(1) << info.vitesse << " km/h";
        assignmentLines.push_back(line.str());
    }

    rapporteur_.exporterFichier(filePath.toStdString(), systeme_.getColis(), systeme_.getTransporteurs(), assignmentLines);
    setStatusMessage("Rapport exporté avec succès");
    QMessageBox::information(this, "Succès", "Rapport exporté avec succès.");
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    saveColisToJson();
    QMainWindow::closeEvent(event);
}
