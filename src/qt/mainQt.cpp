#include "mainWindow.hpp"
#include <QApplication>
#include <QDir>
#include <QCoreApplication> // For applicationDirPath
#include <QMessageBox>

#include "../../include/config.hpp"
#include <filesystem>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // --- Set the base path for application data to the executable's directory ---
    std::filesystem::path appExecDir = std::filesystem::u8path(QCoreApplication::applicationDirPath().toStdString());
    // On macOS, if running from a .app bundle, applicationDirPath() points to YourApp.app/Contents/MacOS/
    // We want the "output" directory to be outside the .app bundle, at the same level as YourApp.app
    // Alternatively, if running the executable directly from the build directory, create output beside it

#ifdef Q_OS_MACOS
    // appExecDir.native() returns the native string representation
    // It can be implicitly converted to std::string
    std::string nativePathStr = appExecDir.native(); // Direct assignment with implicit conversion
    // You could also use static_cast or an explicit constructor, but implicit conversion is usually fine
    // std::string nativePathStr = static_cast<std::string>(appExecDir.native());
    const std::string suffix = ".app/Contents/MacOS";
    if (nativePathStr.size() >= suffix.size() &&
        nativePathStr.rfind(suffix) == nativePathStr.size() - suffix.size())
    {                                                                      // Simple check if running inside a .app bundle
        appExecDir = appExecDir.parent_path().parent_path().parent_path(); // Move up to the directory containing the .app
    }
    else
    {
        // Running the executable directly (e.g., from the build/ directory)
    }
#elif defined(Q_OS_WIN32) || defined(Q_OS_UNIX)
    // On Windows and Linux, the executable's directory is usually a suitable base path
    // No special handling needed
#else
#warning "Unknown platform - directory logic might be incomplete."
#endif

    huffman::DEFAULT_FILE_PATH = appExecDir;
    try
    {
        if (!std::filesystem::exists(huffman::DEFAULT_FILE_PATH / "output"))
            std::filesystem::create_directories(huffman::DEFAULT_FILE_PATH / "output");
        if (!std::filesystem::exists(huffman::DEFAULT_FILE_PATH / "output/tmp"))
            std::filesystem::create_directories(huffman::DEFAULT_FILE_PATH / "output/tmp");
    }
    catch (const std::filesystem::filesystem_error &e)
    {
        QMessageBox::critical(nullptr, "Error", QString("Failed to create application data directories: %1").arg(e.what()));
        return 1;
    }
    MainWindow w;
    w.show();
    return a.exec();
}
