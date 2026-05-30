#pragma once

#include <QMainWindow>
#include <QDate>

#include "include/SystemeLivraison.h"
#include "include/RapportManager.h"

class QComboBox;
class QLineEdit;
class QLabel;
class QTableWidget;
class QPushButton;
class QListWidget;
class QTabWidget;
class QProgressBar;
class QFrame;
class QCloseEvent;

struct AssignmentInfo
{
    QString transporteur;
    double prix = 0.0;
    double vitesse = 0.0;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void onCreateColis();
    void onUpdateStatut();
    void onAssignTransporteur();
    void onExportLogs();

private:
    void buildUi();
    void refreshStats();
    void refreshTable();
    void refreshDailyTable();
    void setStatusMessage(const QString &message, bool isError = false);
    void loadColisFromJson();
    void saveColisToJson() const;

protected:
    void closeEvent(QCloseEvent *event) override;

    QFrame *createStatCard(const QString &title, QLabel *&valueLabel);

    SystemeLivraison systeme_;
    RapportManager rapporteur_;

    QMap<int, AssignmentInfo> assignments_;
    QMap<int, QDate> createdDates_;

    QListWidget *sidebar_ = nullptr;
    QTabWidget *tabs_ = nullptr;
    QLabel *logoLabel_ = nullptr;

    QLabel *totalLabel_ = nullptr;
    QLabel *attenteLabel_ = nullptr;
    QLabel *transitLabel_ = nullptr;
    QLabel *livreLabel_ = nullptr;
    QLabel *annuleLabel_ = nullptr;

    QProgressBar *attenteBar_ = nullptr;
    QProgressBar *transitBar_ = nullptr;
    QProgressBar *livreBar_ = nullptr;
    QProgressBar *annuleBar_ = nullptr;

    QLineEdit *idInput_ = nullptr;
    QLineEdit *poidsInput_ = nullptr;
    QLineEdit *distanceInput_ = nullptr;
    QLineEdit *longueurInput_ = nullptr;
    QLineEdit *largeurInput_ = nullptr;
    QLineEdit *hauteurInput_ = nullptr;
    QLineEdit *destinationInput_ = nullptr;
    QComboBox *typeCombo_ = nullptr;

    QLineEdit *updateIdInput_ = nullptr;
    QComboBox *statutCombo_ = nullptr;

    QLineEdit *assignIdInput_ = nullptr;

    QTableWidget *stockTable_ = nullptr;
    QTableWidget *dailyTable_ = nullptr;
};
