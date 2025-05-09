#include "mainWindow.hpp"
#include "ui_mainWindow.h" // Generated from mainWindow.ui

#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

#include "config.hpp"
#include "file.hpp"
#include "utils.hpp"
#include "table.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // You can set initial states or tooltips here if needed
    // e.g., ui->compressOutputFileEdit->setPlaceholderText("Default: <input_name>.huff in output/");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateStatus(const QString &message, bool isError)
{
    ui->statusLabel->setText(message);
    if (isError)
    {
        // Optionally change color or add to a log
        QMessageBox::critical(this, "Error", message);
    }
    else
    {
        // QMessageBox::information(this, "Info", message); // Can be too intrusive
    }
    qApp->processEvents(); // Ensure UI updates immediately
}

// --- Compress Tab Slots ---
void MainWindow::on_browseCompressInputButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Select Input File to Compress", "", "All Files (*.*)");
    if (!filePath.isEmpty())
    {
        ui->compressInputFileEdit->setText(filePath);
    }
}

void MainWindow::on_browseCompressOutputButton_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this, "Select Output Compressed File", "", "Huffman Compressed File (*.huff);;All Files (*.*)");
    if (!filePath.isEmpty())
    {
        ui->compressOutputFileEdit->setText(filePath);
    }
}

void MainWindow::on_compressButton_clicked()
{
    QString inputFile = ui->compressInputFileEdit->text();
    QString outputFile = ui->compressOutputFileEdit->text();

    if (inputFile.isEmpty())
    {
        updateStatus("Error: Input file for compression is required.", true);
        return;
    }

    huffman::YOUR_INPUT_PATH = inputFile.toStdString();
    huffman::YOUR_INPUT_COMPRESSED_PATH = outputFile.toStdString(); // If empty, your logic will create default

    try
    {
        huffman::writeBaseName(); // Uses YOUR_INPUT_PATH
        huffman::writeExten();    // Uses YOUR_INPUT_PATH
    }
    catch (const std::exception &e)
    {
        updateStatus(QString("Error preparing metadata: ") + e.what(), true);
        return;
    }

    updateStatus("Compressing... Please wait.", false);
    this->setEnabled(false); // Disable window during operation

    try
    {
        huffman::compressFile();

        QString finalOutputPath = QString::fromStdString(huffman::YOUR_INPUT_COMPRESSED_PATH);
        QString outputMessage = "Compression successful!\n";
        QString statusMessage = "";
        try
        {
            statusMessage += "Compressed Path: " + finalOutputPath;
            statusMessage += "\n" + QString::fromStdString(huffman::getRatio());
        }
        catch (const std::runtime_error &e_ratio)
        {
            statusMessage += "\nCould not retrieve compression ratio: " + QString(e_ratio.what());
        }
        updateStatus(statusMessage, false);
        QMessageBox::information(this, "Success", outputMessage);
    }
    catch (const std::ios_base::failure &e)
    {
        updateStatus(QString("File I/O Error during compression: ") + e.what(), true);
    }
    catch (const std::runtime_error &e)
    {
        updateStatus(QString("Runtime Error during compression: ") + e.what(), true);
    }
    catch (const std::exception &e)
    {
        updateStatus(QString("An unexpected error occurred during compression: ") + e.what(), true);
    }
    catch (...)
    {
        updateStatus("An unknown error occurred during compression.", true);
    }
    this->setEnabled(true);
}

// --- Decompress Tab Slots ---
void MainWindow::on_browseDecompressInputButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Select Input File to Decompress", "", "Huffman Compressed File (*.huff);;All Files (*.*)");
    if (!filePath.isEmpty())
    {
        ui->decompressInputFileEdit->setText(filePath);
    }
}

void MainWindow::on_browseDecompressOutputButton_clicked()
{
    // For decompression, the default output name depends on stored metadata (basename, exten)
    // So, getSaveFileName might not be ideal if we want to show the default suggestion.
    // For simplicity, we'll let the user choose or leave blank for default.
    QString filePath = QFileDialog::getSaveFileName(this, "Select Output Decompressed File", "", "All Files (*.*)");
    if (!filePath.isEmpty())
    {
        ui->decompressOutputFileEdit->setText(filePath);
    }
}

void MainWindow::on_decompressButton_clicked()
{
    QString inputFile = ui->decompressInputFileEdit->text();
    QString outputFile = ui->decompressOutputFileEdit->text();

    if (inputFile.isEmpty())
    {
        updateStatus("Error: Input file for decompression is required.", true);
        return;
    }

    // IMPORTANT: For decompression, YOUR_INPUT_PATH is the *compressed* file.
    huffman::YOUR_INPUT_PATH = inputFile.toStdString();
    huffman::YOUR_INPUT_DECOMPRESSED_PATH = outputFile.toStdString(); // If empty, your logic will create default

    updateStatus("Decompressing... Please wait.", false);
    this->setEnabled(false); // Disable window during operation

    try
    {
        huffman::decompressFile();

        QString finalOutputPath = QString::fromStdString(huffman::YOUR_INPUT_DECOMPRESSED_PATH);
        QString outputMessage = "Decompression successful!\n";
        QString statusMessage = "Deompressed Path: " + finalOutputPath;
        updateStatus(statusMessage, false);
        QMessageBox::information(this, "Success", outputMessage);
    }
    catch (const std::ios_base::failure &e)
    {
        updateStatus(QString("File I/O Error during decompression: ") + e.what(), true);
    }
    catch (const std::runtime_error &e)
    {
        updateStatus(QString("Runtime Error during decompression: ") + e.what(), true);
    }
    catch (const std::exception &e)
    {
        updateStatus(QString("An unexpected error occurred during decompression: ") + e.what(), true);
    }
    catch (...)
    {
        updateStatus("An unknown error occurred during decompression.", true);
    }
    this->setEnabled(true);
}