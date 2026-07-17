
#include "SensorsView.hpp"

LEDView::LEDView(QWidget *p, QString titleText) : QWidget(p) {
    layout = new QVBoxLayout(this);
    title = new QLabel(this);
    display = new QLabel(this);
    subtitle = new QLabel(this);


    QPalette pal = QPalette();
    auto color = pal.color(QPalette::Window).lighter(125);
    setObjectName("ledRoot");
    setAttribute(Qt::WA_StyledBackground, true);
    setStyleSheet(QString("#ledRoot { border-radius: 12px; background-color: %1; }").arg(color.name()));

    setAutoFillBackground(true);
    title->setText(titleText);
    display->setText("--");
    display->setStyleSheet("font-size: 48px; font-weight: bold; ");

    layout->setSpacing(12);

    auto topleft = Qt::AlignTop | Qt::AlignLeft;
    layout->addWidget(title, 0, topleft);
    layout->addWidget(display, 0, topleft);
    layout->addWidget(subtitle, 0, topleft);
}

Sensors::Sensors(QWidget *p) : QWidget(p) {
    this->slot1 = new LEDView(this, "RAM");
    this->slot2 = new LEDView(this, "CPU");
    this->layout = new QGridLayout(this);

    slot1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    slot2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    layout->setColumnStretch(0, 1);
    layout->setColumnStretch(1, 1);
    layout->addWidget(slot1, 0, 0);
    layout->addWidget(slot2, 0, 1);
    layout->setContentsMargins(12, 12, 12, 12);

}

