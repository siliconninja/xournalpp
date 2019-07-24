/*
 * Xournal++
 *
 * This file is part of the Xournal UnitTests
 *
 * @author Xournal++ Team
 * https://github.com/xournalpp/xournalpp
 *
 * @license GNU GPLv2 or later
 */

#include "control/xojfile/LoadHandler.h"
#include <config-test.h>

#ifdef TEST_CHECK_SPEED
#include "SpeedTest.cpp"
#endif

#include <cppunit/extensions/HelperMacros.h>

#include <stdlib.h>

class LoadHandlerTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(LoadHandlerTest);

#ifdef TEST_CHECK_SPEED
	CPPUNIT_TEST(testSpeed);
#endif

	CPPUNIT_TEST(testLoad);
	CPPUNIT_TEST(testLoadZipped);
	CPPUNIT_TEST(testLoadUnzipped);

	CPPUNIT_TEST(testPages);
	CPPUNIT_TEST(testPagesZipped);
	CPPUNIT_TEST(testPageType);
	CPPUNIT_TEST(testPageTypeZipped);
	CPPUNIT_TEST(testLayer);
	CPPUNIT_TEST(testLayerZipped);
	CPPUNIT_TEST(testText);
	CPPUNIT_TEST(testTextZipped);
	CPPUNIT_TEST(testStroke);
	CPPUNIT_TEST(loadImage);

	CPPUNIT_TEST_SUITE_END();

public:
	void setUp()
	{
	}

	void tearDown()
	{
	}

#ifdef TEST_CHECK_SPEED
	void testSpeed()
	{
		SpeedTest speed;
		speed.startTest("document load");
		
		LoadHandler handler;
		handler.loadDocument(GET_TESTFILE("big-test.xoj"));
		
		speed.endTest();
	}

	void testSpeedZipped()
	{
		SpeedTest speed;
		speed.startTest("document load");

		LoadHandler handler;
		handler.loadDocument(GET_TESTFILE("packaged_xopp/big-test.xopp"));

		speed.endTest();
	}
#endif

	void testLoad()
	{
		LoadHandler handler;
		Document* doc = handler.loadDocument(GET_TESTFILE("test1.xoj"));

		CPPUNIT_ASSERT_EQUAL((size_t) 1, doc->getPageCount());
		PageRef page = doc->getPage(0);

		CPPUNIT_ASSERT_EQUAL((size_t) 1, (*page).getLayerCount());
		Layer* layer = (*(*page).getLayers())[0];

		Element* element = (*layer->getElements())[0];
		CPPUNIT_ASSERT_EQUAL(ELEMENT_TEXT, element->getType());

		Text* text = (Text*) element;

		CPPUNIT_ASSERT_EQUAL(string("12345"), text->getText());
	}

	void testLoadZipped()
	{
		LoadHandler handler;
		Document* doc = handler.loadDocument(GET_TESTFILE("packaged_xopp/test.xopp"));

		CPPUNIT_ASSERT_EQUAL((size_t) 1, doc->getPageCount());
		PageRef page = doc->getPage(0);

		CPPUNIT_ASSERT_EQUAL((size_t) 1, (*page).getLayerCount());
		Layer* layer = (*(*page).getLayers())[0];

		Element* element = (*layer->getElements())[0];
		CPPUNIT_ASSERT_EQUAL(ELEMENT_TEXT, element->getType());

		Text* text = (Text*) element;

		CPPUNIT_ASSERT_EQUAL(string("12345"), text->getText());
	}

	void testLoadUnzipped()
	{
		LoadHandler handler;
		Document* doc = handler.loadDocument(GET_TESTFILE("test1.unzipped.xoj"));

		CPPUNIT_ASSERT_EQUAL((size_t) 1, doc->getPageCount());
		PageRef page = doc->getPage(0);

		CPPUNIT_ASSERT_EQUAL((size_t) 1, (*page).getLayerCount());
		Layer* layer = (*(*page).getLayers())[0];

		Element* element = (*layer->getElements())[0];
		CPPUNIT_ASSERT_EQUAL(ELEMENT_TEXT, element->getType());

		Text* text = (Text*) element;

		CPPUNIT_ASSERT_EQUAL(string("12345"), text->getText());
	}

	void testPages()
	{
		LoadHandler handler;
		Document* doc = handler.loadDocument(GET_TESTFILE("load/pages.xoj"));

		CPPUNIT_ASSERT_EQUAL((size_t) 6, doc->getPageCount());
	}

	void testPagesZipped()
	{
		LoadHandler handler;
		Document* doc = handler.loadDocument(GET_TESTFILE("packaged_xopp/pages.xopp"));

		CPPUNIT_ASSERT_EQUAL((size_t) 6, doc->getPageCount());
	}

	void checkPageType(Document* doc, int pageIndex, string expectedText, PageType expectedBgType)
	{
		PageRef page = doc->getPage(pageIndex);

		PageType bgType = page->getBackgroundType();
		CPPUNIT_ASSERT(expectedBgType == bgType);

		CPPUNIT_ASSERT_EQUAL((size_t) 1, (*page).getLayerCount());
		Layer* layer = (*(*page).getLayers())[0];

		Element* element = (*layer->getElements())[0];
		CPPUNIT_ASSERT_EQUAL(ELEMENT_TEXT, element->getType());

		Text* text = (Text*) element;
		CPPUNIT_ASSERT_EQUAL(expectedText, text->getText());
	}

	void testPageType()
	{
		LoadHandler handler;
		Document* doc = handler.loadDocument(GET_TESTFILE("load/pages.xoj"));

		CPPUNIT_ASSERT_EQUAL((size_t) 6, doc->getPageCount());
		checkPageType(doc, 0, "p1", PageType(PageTypeFormat::Plain));
		checkPageType(doc, 1, "p2", PageType(PageTypeFormat::Ruled));
		checkPageType(doc, 2, "p3", PageType(PageTypeFormat::Lined));
		checkPageType(doc, 3, "p4", PageType(PageTypeFormat::Staves));
		checkPageType(doc, 4, "p5", PageType(PageTypeFormat::Graph));
		checkPageType(doc, 5, "p6", PageType(PageTypeFormat::Image));
	}

	void testPageTypeZipped()
	{
		LoadHandler handler;
		Document* doc = handler.loadDocument(GET_TESTFILE("packaged_xopp/pages.xopp"));

		CPPUNIT_ASSERT_EQUAL((size_t) 6, doc->getPageCount());
		checkPageType(doc, 0, "p1", PageType(PageTypeFormat::Plain));
		checkPageType(doc, 1, "p2", PageType(PageTypeFormat::Ruled));
		checkPageType(doc, 2, "p3", PageType(PageTypeFormat::Lined));
		checkPageType(doc, 3, "p4", PageType(PageTypeFormat::Staves));
		checkPageType(doc, 4, "p5", PageType(PageTypeFormat::Graph));
		checkPageType(doc, 5, "p6", PageType(PageTypeFormat::Image));
	}

	void checkLayer(PageRef page, int layerIndex, string expectedText)
	{
		Layer* layer = (*(*page).getLayers())[layerIndex];

		Element* element = (*layer->getElements())[0];
		CPPUNIT_ASSERT_EQUAL(ELEMENT_TEXT, element->getType());

		Text* text = (Text*) element;
		CPPUNIT_ASSERT_EQUAL(expectedText, text->getText());
	}

	void testLayer()
	{
		LoadHandler handler;
		Document* doc = handler.loadDocument(GET_TESTFILE("load/layer.xoj"));

		CPPUNIT_ASSERT_EQUAL((size_t) 1, doc->getPageCount());
		PageRef page = doc->getPage(0);

		CPPUNIT_ASSERT_EQUAL((size_t) 3, (*page).getLayerCount());
		checkLayer(page, 0, "l1");
		checkLayer(page, 1, "l2");
		checkLayer(page, 2, "l3");
	}

	void testLayerZipped()
	{
		LoadHandler handler;
		Document* doc = handler.loadDocument(GET_TESTFILE("packaged_xopp/layer.xopp"));

		CPPUNIT_ASSERT_EQUAL((size_t) 1, doc->getPageCount());
		PageRef page = doc->getPage(0);

		CPPUNIT_ASSERT_EQUAL((size_t) 3, (*page).getLayerCount());
		checkLayer(page, 0, "l1");
		checkLayer(page, 1, "l2");
		checkLayer(page, 2, "l3");
	}

	void testText()
	{
		LoadHandler handler;
		Document* doc = handler.loadDocument(GET_TESTFILE("load/text.xml"));

		CPPUNIT_ASSERT_EQUAL((size_t) 1, doc->getPageCount());
		PageRef page = doc->getPage(0);

		CPPUNIT_ASSERT_EQUAL((size_t) 1, (*page).getLayerCount());
		Layer* layer = (*(*page).getLayers())[0];

		Text* t1 = (Text*)(*layer->getElements())[0];
		CPPUNIT_ASSERT_EQUAL(ELEMENT_TEXT, t1->getType());

		Text* t2 = (Text*)(*layer->getElements())[1];
		CPPUNIT_ASSERT_EQUAL(ELEMENT_TEXT, t2->getType());

		Text* t3 = (Text*)(*layer->getElements())[2];
		CPPUNIT_ASSERT_EQUAL(ELEMENT_TEXT, t3->getType());

		CPPUNIT_ASSERT_EQUAL(string("red"), t1->getText());
		CPPUNIT_ASSERT_EQUAL(string("blue"), t2->getText());
		CPPUNIT_ASSERT_EQUAL(string("green"), t3->getText());

		CPPUNIT_ASSERT_EQUAL(0xff0000, t1->getColor());
		CPPUNIT_ASSERT_EQUAL(0x3333CC, t2->getColor());
		CPPUNIT_ASSERT_EQUAL(0x00f000, t3->getColor());
	}

	void testTextZipped()
	{
		LoadHandler handler;
		Document* doc = handler.loadDocument(GET_TESTFILE("packaged_xopp/text.xopp"));

		CPPUNIT_ASSERT_EQUAL((size_t) 1, doc->getPageCount());
		PageRef page = doc->getPage(0);

		CPPUNIT_ASSERT_EQUAL((size_t) 1, (*page).getLayerCount());
		Layer* layer = (*(*page).getLayers())[0];

		Text* t1 = (Text*)(*layer->getElements())[0];
		CPPUNIT_ASSERT_EQUAL(ELEMENT_TEXT, t1->getType());

		Text* t2 = (Text*)(*layer->getElements())[1];
		CPPUNIT_ASSERT_EQUAL(ELEMENT_TEXT, t2->getType());

		Text* t3 = (Text*)(*layer->getElements())[2];
		CPPUNIT_ASSERT_EQUAL(ELEMENT_TEXT, t3->getType());

		CPPUNIT_ASSERT_EQUAL(string("red"), t1->getText());
		CPPUNIT_ASSERT_EQUAL(string("blue"), t2->getText());
		CPPUNIT_ASSERT_EQUAL(string("green"), t3->getText());

		CPPUNIT_ASSERT_EQUAL(0xff0000, t1->getColor());
		CPPUNIT_ASSERT_EQUAL(0x3333CC, t2->getColor());
		CPPUNIT_ASSERT_EQUAL(0x00f000, t3->getColor());
	}

	void testStroke()
	{

	}

	void loadImage()
	{

	}

};

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION(LoadHandlerTest);
