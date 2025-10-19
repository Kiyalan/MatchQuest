#pragma once
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

class Level;
class PlayerController;

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public:
    static MainWindow* getInstance();
protected:
    // 当 view 与 window 大小不匹配时居中显示
    QLayout* layout;
private:
    Level* level = nullptr;
    PlayerController* player = nullptr;
    // 在不同关卡之间切换，清除并解绑上一个 level 的资源，分配并组织下一个 level 的资源
public:
    int inputWindowWidth = 10, inputWindowHeight = 10;
    QString inputLevelPath;
public slots:
    void openStartMenuLevel();
    void openSinglePlayerLevel();
    void openDualPlayerLevel();
    void loadLevelFromFile();
private:
    void bindPlayerToLevel();
protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
};



#endif // MAINWINDOW_H
