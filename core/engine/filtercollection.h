#ifndef FILTERCOLLECTION_H
#define FILTERCOLLECTION_H

// auto filters
#include "vfx/gldeinterlace.h"
#include "vfx/glpadding.h"
#include "vfx/glresize.h"
#include "vfx/glorientation.h"

// video filters
#include "vfx/glbackgroundcolor.h"
#include "vfx/glblur.h"
#include "vfx/glborder.h"
#include "vfx/glcrop.h"
#include "vfx/glcut.h"
#include "vfx/gldeconvolutionsharpen.h"
#include "vfx/gldiffusion.h"
#include "vfx/gldefish.h"
#include "vfx/gldropshadow.h"
#include "vfx/gledge.h"
#include "vfx/glglow.h"
#include "vfx/glliftgammagain.h"
#include "vfx/glopacity.h"
#include "vfx/glpixelize.h"
#include "vfx/glsaturation.h"
#include "vfx/glsharpen.h"
#include "vfx/glsize.h"
#include "vfx/glsoftborder.h"
#include "vfx/glvignette.h"
#include "vfx/glwater.h"
#include "vfx/glwhitebalance.h"
#include "vfx/gltest.h"

// video compositions
#include "vfx/glmix.h"
#include "vfx/gloverlay.h"

// audio filters
#include "afx/audiovolume.h"

// audio compositions
#include "afx/audiocopy.h"
#include "afx/audiomix.h"
#include "afx/audiocrossfade.h"



template <class T>
class Maker
{
public:
	static Filter* make( QString id, QString name ) { return new T( id, name ); }
};



class FilterEntry
{
public:
	FilterEntry( QString id, QString n, Filter* (*p)(QString,QString) )
		: identifier( id ),
		name( n ),
		makeFilter( p )
	{}
	
	QSharedPointer<Filter> create() { return QSharedPointer<Filter>( makeFilter( identifier, name ) ); }

	QString identifier; // must be unique
	QString name; // translated UI name

private:
	Filter* (*makeFilter)(QString,QString);
};



class FilterCollection
{
public:
	FilterCollection();
	static FilterCollection* getGlobalInstance();
	
	QList<FilterEntry> videoFilters;
	QList<FilterEntry> sourceVideoFilters;
	QList<FilterEntry> videoTransitions;
	QList<FilterEntry> audioFilters;
	QList<FilterEntry> sourceAudioFilters;
	QList<FilterEntry> audioTransitions;
};

#endif // FILTERCOLLECTION_H
