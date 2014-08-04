#include <QLabel>

#include "engine/filtercollection.h"
#include "gui/fxpage.h"



FxPage::FxPage()
{
	setupUi( this );
	
	currentEffectsWidget = NULL;
	
	FilterCollection *fc = FilterCollection::getGlobalInstance();
	int i;
	
	for ( i = 0; i < fc->videoFilters.count(); ++i ) {
		QListWidgetItem *it = new QListWidgetItem( fc->videoFilters[ i ].name );
		it->setData( 100, fc->videoFilters[ i ].identifier );
		listWidget->addItem( it );
	}
}



void FxPage::clipSelected( Clip *clip )
{
	if ( currentEffectsWidget ) {
		delete currentEffectsWidget;
		currentEffectsWidget = NULL;
		filterWidgets.clear();
	}
	
	if ( clip ) {
		int i, j = 0;
		currentEffectsWidget = new QWidget();
		currentEffectsWidget->setMinimumWidth( 150 );
		QGridLayout *effectsWidgetLayout = new QGridLayout( currentEffectsWidget );
		effectsWidgetLayout->setContentsMargins( 0, 0, 0, 0 );
		for ( i = 0; i < clip->videoFilters.count(); ++i ) {
			FilterWidget *fw = new FilterWidget( currentEffectsWidget, clip, clip->videoFilters.at( i ) );
			connect( fw, SIGNAL(filterDeleted(Clip*,Filter*)), this, SLOT(deletedFilter(Clip*,Filter*)) );
			connect( fw, SIGNAL(updateFrame()), this, SIGNAL(updateFrame()) );
			connect( fw, SIGNAL(editAnimation(FilterWidget*,ParameterWidget*,Parameter*)), this, SIGNAL(editAnimation(FilterWidget*,ParameterWidget*,Parameter*)) );
			effectsWidgetLayout->addWidget( fw, i, 1 );
			filterWidgets.append( fw );
		}
		effectsWidgetLayout->setRowStretch( i, 1 );
		effectsWidget->setWidget( currentEffectsWidget );
	}
}



void FxPage::deletedFilter( Clip *c, Filter *f )
{
	int i;
	for ( i = 0; i < filterWidgets.count(); ++i ) {
		if ( filterWidgets[i]->getFilter() == f ) {
			emit filterDeleted( c, f );
			delete filterWidgets.takeAt( i );
			break;
		}
	}
}
