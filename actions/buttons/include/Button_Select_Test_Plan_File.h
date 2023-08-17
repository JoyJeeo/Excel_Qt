#ifndef BUTTON_SELECT_TEST_PLAN_FILE_H
#define BUTTON_SELECT_TEST_PLAN_FILE_H

#include <QPushButton>

class Button_Select_Test_Plan_File : public QPushButton
{
    Q_OBJECT

public:
    Button_Select_Test_Plan_File(QWidget *parent = nullptr);
    ~Button_Select_Test_Plan_File();

    void total_task();

public slots:
    void task_select_test_plan();

private:


};

#endif // BUTTON_SELECT_TEST_PLAN_FILE_H
