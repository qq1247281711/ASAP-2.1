#ifndef PATHOLOGYWORKSTATION_H
#define PATHOLOGYWORKSTATION_H

#include "asaplib_export.h"
#include <QMainWindow>
#include <QDir>
#include <QStringList>
#include <memory>

class MultiResolutionImage;
class QHBoxLayout;
class QAction;
class QStatusBar;
class QMenuBar;
class PathologyViewer;
class WorkstationExtensionPluginInterface;
class QActionGroup;
class QSettings;
class QKeyPressEvent;

/*

OWNERSHIP

Workstation is the owner of all the PathologyViews
Workstation is the owner of all Plugins, also the ToolPlugins generated by ExtensionPlugins
Workstation is the owner of the MultiResolutionImage

Viewer is the owner of RenderThread, TileManager, ScaleBar, MiniMap, RenderWorkers and the Scene

*/

class ASAPLIB_EXPORT ASAP_Window : public QMainWindow
{
  Q_OBJECT

public:
  explicit ASAP_Window(QWidget* parent = 0);
  ~ASAP_Window();

  //设置缓存大小
  void setCacheSize(const unsigned long long& cacheMaxByteSize);

  //获取缓存大小
  unsigned long long getCacheSize() const;

  //获取文件名和格式
  QList<QString> getFileNameAndFactory();

  //打开文件
  void openFile(const QString& fileName, const QString& factoryName = QString("default"));

signals:
  //新图片加载
  void newImageLoaded(std::weak_ptr<MultiResolutionImage>, const std::string&);
  //关闭图片
  void imageClosed();

private slots:
  //关闭事件点击
  void on_actionClose_triggered();
  //打开事件点击
  void on_actionOpen_triggered();
  //关于事件点击
  void on_actionAbout_triggered();

private:
  //共享库的扩展
  static const char* sharedLibraryExtensions;
  //多分辨率图像
  //shared_ptr是一种智能指针（smart pointer），作用有如同指针，但会记录有多少个shared_ptrs共同指向一个对象。这便是所谓的引用计数（reference counting）。
  std::shared_ptr<MultiResolutionImage> _img;
  unsigned long long _cacheMaxByteSize;
  //QSettings使用户可以保存应用程序设置，并且支持用户自定义存储格式。
  //QSetings API基于QVariant,因而你可以存储却大部分类型的数据
  QSettings* _settings;

  void closeEvent(QCloseEvent *event);
  
  // Plugins 插件
  QDir _pluginsDir;
  std::vector<std::string> _extensionPluginFileNames;
  std::vector<std::string> _toolPluginFileNames;

  // GUI object
  QAction *actionOpen;
  QAction *actionClose;
  QAction *actionAbout;
  QActionGroup *_toolActions;
  QWidget *centralWidget;
  QHBoxLayout *horizontalLayout_2;
  PathologyViewer *pathologyView;
  QMenuBar *menuBar;
  QMenu *menuFile;
  QMenu *menuView;
  QMenu *menuHelp;
  QToolBar *mainToolBar;
  QStatusBar *statusBar;

  // Initialize the GUI 初始化GUI
  void initializeDocks();
  void setupUi();
  void retranslateUi();
  void showShortcutOverview();
  void loadPlugins();
  void readSettings();
  void writeSettings();

  virtual void keyPressEvent(QKeyEvent* event);

  std::vector<std::unique_ptr<WorkstationExtensionPluginInterface> > _extensions;
};

#endif // PATHOLOGYWORKSTATION_H
