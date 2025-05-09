#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Compress Tab
    void on_browseCompressInputButton_clicked();
    void on_browseCompressOutputButton_clicked();
    void on_compressButton_clicked();

    // Decompress Tab
    void on_browseDecompressInputButton_clicked();
    void on_browseDecompressOutputButton_clicked();
    void on_decompressButton_clicked();

private:
    Ui::MainWindow *ui; // Pointer to the UI components

    // Helper function to update status
    void updateStatus(const QString &message, bool isError = false);
};

#endif