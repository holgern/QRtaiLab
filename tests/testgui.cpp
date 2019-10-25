 #include <QtTest/QtTest>
#include "../src/qrtailab.h"
#include "../src/meters_manager.h"
#include "../src/leds_manager.h"
#include "../src/scopes_manager.h"
#include "../src/parameters_manager.h"
#include "../src/main_window.h"




class TestQrtaiLab : public QObject
{
   Q_OBJECT
private:
 TargetThread* targetthread;
  QRL_MainWindow mainWin;

   private slots:
	void initTestCase();
	void testProfile();
	void cleanupTestCase();
};
void TestQrtaiLab::initTestCase()
{
 mainWin.show();
 targetthread=mainWin.targetthread;
 targetthread->setVerbose(0);
}



void TestQrtaiLab::testProfile()
{
	unsigned int expected;
	 QCOMPARE(mainWin.isVisible(),true);
	targetthread=mainWin.targetthread;
	targetthread->setVerbose(0);
	
	emit mainWin.connectDialog();
	QTest::qWait(100);
	expected=1;
	QCOMPARE(targetthread->getIsTargetConnected(),expected);

	

	emit mainWin.saveProfile();
	
	targetthread->disconnectFromTarget();
	expected=0;
	QCOMPARE(targetthread->getIsTargetConnected(),expected);
	emit mainWin.connect_WProfile();
	expected=1;
	QCOMPARE(targetthread->getIsTargetConnected(),expected);
	
	
	targetthread->disconnectFromTarget();
	expected=0;
	QCOMPARE(targetthread->getIsTargetConnected(),expected);
}

void TestQrtaiLab::cleanupTestCase()
{

	mainWin.close();

}
 QTEST_MAIN(TestQrtaiLab)
 #include "testgui.moc"
