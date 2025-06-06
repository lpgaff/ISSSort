#ifndef __ISSEVTS_HH
#define __ISSEVTS_HH

#include <iostream>
#include <vector>
#include <string>
#include <memory>

#include "TVector2.h"
#include "TVector3.h"
#include "TRandom3.h"
#include "TObject.h"

// Some physical detector geometry for array
const double wafer_length = 125.0;	///< total length of silicon (mm)
const double wafer_gap = 0.5;		///< gap between silicon wafers (mm)
const double wafer_guard = 1.508;	///< width of the inactive region around edge of silicon (mm)
const double pstrip_pitch = 0.953;	///< pitch of a single p-side strip (mm)
const double nstrip_pitch = 2.0;	///< pitch of a single n-side strip (mm)
const double array_radius = 27.0;	///< radius to top surface of silicon (mm)

// Some physical geometry of the CD
const double cd_inner_radius = 11.0;	///< radius of hole in centre of S3 (mm)
const double cd_ring_width = 0.886;		///< width of the active area of the rings (mm)
const double cd_ring_gap = 0.114;		///< width of the dead area between the rings (mm)
const double cd_sector_pitch = 11.00;	///< pitch of the active area of the sectors (degrees)
const double cd_sector_gap = 0.25;		///< pitch of the dead area between the sectors (degrees)
const double cd_phi_offset = 180.0;		///< angle of the centre of sector 0 with respect to the vertical (degrees)

class ISSArrayEvt : public TObject {

public:

	// setup functions
	ISSArrayEvt();
	~ISSArrayEvt();

	// Event reconstruction
	void SetEvent( float mypen, float mynen,
				  unsigned char mypid, unsigned char mynid,
				  double myptime, double myntime,
				  bool myphit, bool mynhit,
				  unsigned char mymod, unsigned char myrow );

	// Copy event
	void CopyEvent( std::shared_ptr<ISSArrayEvt> in );

	// Return functions
	inline float 			GetEnergy(){ return pen; };
	inline float 			GetPEnergy(){ return pen; };
	inline float 			GetNEnergy(){ return nen; };
	inline unsigned char	GetPID(){ return pid; };
	inline unsigned char	GetNID(){ return nid; };
	inline double			GetTime(){ return ptime; };
	inline double			GetTimeDouble(){ return ptime; };
	inline double			GetPTime(){ return ptime; };
	inline double	 		GetNTime(){ return ntime; };
	inline bool 			GetPHit(){ return phit; };
	inline bool 			GetNHit(){ return nhit; };
	inline unsigned char	GetModule(){ return mod; };
	inline unsigned char	GetRow(){ return row; };

	float		GetX();
	float		GetY();
	float		GetZ();
	float		GetPhi();
	TVector2	GetPhiXY();
	TVector3	GetPosition();

	static int	FindPID( double z );
	static int	FindNID( double phi );
	static int	FindModule( double phi );
	static int	FindRow( double z );
	static int	FindModule( unsigned short detNo );
	static int	FindRow( unsigned short detNo );
	static int	FindAsicP( unsigned short detNo );
	static int	FindAsicN( unsigned short detNo );


protected:

	// variables for particle event
	bool			phit;	///< hit bit value for p-side event
	bool			nhit;	///< hit bit value for n-side event
	unsigned char	mod;	///< module number
	unsigned char	row;	///< row number of the silicon
	float			pen;	///< p-side energy in keV
	float 			nen;	///< n-side energy in keV
	unsigned char	pid;	///< p-side strip id, from 0 - 511, i.e along all 4 wafers
	unsigned char	nid;	///< n-side strip id, from 0 - 65, i.e. around all 6 sides
	double			ptime;	///< p-side timestamp
	double			ntime;	///< n-side timestamp


	ClassDef( ISSArrayEvt, 5 )

};

class ISSArrayPEvt : public ISSArrayEvt {

public:

	// setup functions
	ISSArrayPEvt();
	~ISSArrayPEvt();

protected:

	ClassDef( ISSArrayPEvt, 1 )

};

class ISSSingleLayerDetectorEvt : public TObject {

public:

	// setup functions
	ISSSingleLayerDetectorEvt();
	~ISSSingleLayerDetectorEvt();

	void SetEvent( float myenergy, unsigned char myid,
				  unsigned char mysec, double mytime );

	inline void SetEnergy( float e ){ energy = e; };
	inline void SetSector( unsigned char s ){ sec = s; };
	inline void SetID( unsigned char i ){ id = i; };
	inline void SetTime( double t ){ time = t; };

	inline float			GetEnergy(){ return energy; };
	inline unsigned char	GetID(){ return id; };
	inline unsigned char	GetSector(){ return sec; };
	inline double			GetTime(){ return time; };


protected:

	float			energy;	///< Energy in the  detector
	unsigned char	id;		///< ID list, example: many CeBr detectors of the ScintArray all have different IDs
	unsigned char	sec;	///< sector or quandrant, example: ELUM detector, i.e. 0-3 when split into 4
	double			time;	///< time stamp of the  event

	ClassDef( ISSSingleLayerDetectorEvt, 1 );

};



class ISSMultiLayerDetectorEvt : public TObject {

public:

	// setup functions
	ISSMultiLayerDetectorEvt();
	~ISSMultiLayerDetectorEvt();

	void SetEvent( std::vector<float> myenergy,
				  std::vector<unsigned char> myid,
				  unsigned char mysec, unsigned char myring,
				  double mydetime, double myetime );

	void ClearEvent();

	inline void AddFragment( float myenergy, unsigned char myid ){
		energy.push_back( myenergy );
		id.push_back( myid );
	};

	inline void SetSector( unsigned char s ){ sec = s; };
	inline void SetdETime( double t ){ detime = t; };
	inline void SetETime( double t ){ etime = t; };
	inline void SetRing( unsigned char s ){ ring = s; };

	inline unsigned char	GetDepth(){ return energy.size(); };
	inline unsigned char	GetSector(){ return sec; };
	inline double			GetTime(){ return detime; };
	inline double			GetdETime(){ return detime; };
	inline double			GetETime(){ return etime; };
	inline unsigned char	GetRing(){ return ring; };

	inline std::vector<float>			GetEnergies(){ return energy; };
	inline std::vector<unsigned char>	GetIDs(){ return id; };


	inline float GetEnergy( unsigned char i ){
		if( i < energy.size() ) return energy.at(i);
		else return 0;
	};

	inline float GetEnergyLoss( unsigned char start = 0, unsigned char stop = 0 ){
		float total = 0;
		for( unsigned int j = 0; j < energy.size(); ++j )
			if( GetID(j) >= start && GetID(j) <= stop )
				total += energy.at(j);
		return total;
	};

	inline float GetEnergyRest( unsigned char start = 1, unsigned char stop = 1 ){
		float total = 0;
		for( unsigned int j = 0; j < energy.size(); ++j )
			if( GetID(j) >= start && GetID(j) <= stop )
				total += energy.at(j);
		return total;
	};

	inline float GetEnergyTotal( unsigned char start = 0, unsigned char stop = 1 ){
		float total = 0;
		for( unsigned int j = 0; j < energy.size(); ++j )
			if( GetID(j) >= start && GetID(j) <= stop )
				total += energy.at(j);
		return total;
	};

	inline unsigned char GetID( unsigned char i ){
		if( i < id.size() ) return id.at(i);
		else return -1;
	};


protected:

	// variables for recoil events
	std::vector<float>			energy;	///< differential energy list, i.e. Silicon dE-E length = 2
	std::vector<unsigned char>	id;		///< differential id list, i.e. dE = 0, E = 1, for example
	unsigned char				sec;	///< sector of the recoil detector, i.e 0-3 for QQQ1 quadrants
	double						detime;	///< time stamp of dE event
	double						etime;	///< time stamp of E event

	// extra variables for CD events
	unsigned char				ring;	///< ring of the CD detector

	ClassDef( ISSMultiLayerDetectorEvt, 2 )

};

class ISSRecoilEvt : public ISSMultiLayerDetectorEvt {

public:

	// setup functions
	ISSRecoilEvt();
	~ISSRecoilEvt();

protected:

	ClassDef( ISSRecoilEvt, 5 )

};

class ISSMwpcEvt : public TObject {

public:

	// setup functions
	ISSMwpcEvt();
	~ISSMwpcEvt();

	void SetEvent( int mytacdiff, unsigned char myaxis,
				  double mytime );

	inline void SetTacDiff( int t ){ tacdiff = t; };
	inline void SetAxis( unsigned char a ){ axis = a; };
	inline void SetTime( double t ){ time = t; };

	inline int				GetTacDiff(){ return tacdiff; };
	inline unsigned char	GetAxis(){ return axis; };
	inline double			GetTime(){ return time; };


protected:

	int				tacdiff;	///< TAC differences
	unsigned char	axis;		///< axis ID, usually just x=0 and y=1
	double			time;		///< time stamp of the MWPC event

	ClassDef( ISSMwpcEvt, 2 );

};

class ISSElumEvt : public ISSSingleLayerDetectorEvt {

public:

	// setup functions
	ISSElumEvt();
	~ISSElumEvt();

protected:

	ClassDef( ISSElumEvt, 4 );

};

class ISSZeroDegreeEvt : public ISSMultiLayerDetectorEvt {

public:

	// setup functions
	ISSZeroDegreeEvt();
	~ISSZeroDegreeEvt();

protected:

	ClassDef( ISSZeroDegreeEvt, 5 )

};

class ISSGammaRayEvt : public ISSSingleLayerDetectorEvt {

public:

	// setup functions
	ISSGammaRayEvt();
	~ISSGammaRayEvt();

	void SetEvent( float myenergy, unsigned char myid,
				  unsigned char mysec, unsigned char mytype,
				  double mytime );

	inline void				SetType( unsigned char t ){ type = t; };
	inline unsigned char	GetType(){ return type; };


protected:

	// Extra variables for gamma-ray events
	unsigned char	type;	///< Detector type: 0 - ScintArray, 1 - ... HPGe?

	ClassDef( ISSGammaRayEvt, 3 );

};

class ISSLumeEvt : public TObject {

public:

	// setup information
	ISSLumeEvt();
	~ISSLumeEvt();

	void SetEvent( float myenergy, unsigned char myid,
				  double mytime, float myn, float myf );

	inline void SetID( unsigned char i ){ id = i; };

	inline void SetBE( float e ){ be = e; }
	inline void SetNE( float n ){ ne = n; }
	inline void SetFE( float f ){ fe = f; }

	inline void SetTime( double t ){ time = t; };
	inline void SetX( float xx ){ x = xx; }

	inline unsigned char	GetID(){ return id; };

	inline float			GetBE(){ return be; };
	inline float			GetNE(){ return ne; };
	inline float			GetFE(){ return fe; };

	inline double			GetTime(){ return time; };
	inline float			GetX(){ return x; };

protected:
	unsigned char id;		///< Detector ID

	float be;			///< Energy in the detector (from the back-plane)
	float ne;			///< near-side energy
	float fe;			///< far-side energy


	float x;			///< position along the detector
	double time;			///< time stamp of the event

	ClassDef( ISSLumeEvt, 2 );

};

class ISSCDEvt : public ISSMultiLayerDetectorEvt {

public:

	ISSCDEvt();
	~ISSCDEvt();

	// Geometry
	float		GetX( bool randomised = false );
	float		GetY( bool randomised = false );
	float		GetPhi( bool randomised = false );
	TVector2	GetPhiXY( bool randomised = false );


protected:

	ClassDef( ISSCDEvt, 4 );

};



class ISSEvts : public TObject {

public:

	// setup functions
	ISSEvts();
	~ISSEvts();

	void AddEvt( std::shared_ptr<ISSArrayEvt> event );
	void AddEvt( std::shared_ptr<ISSArrayPEvt> event );
	void AddEvt( std::shared_ptr<ISSRecoilEvt> event );
	void AddEvt( std::shared_ptr<ISSMwpcEvt> event );
	void AddEvt( std::shared_ptr<ISSElumEvt> event );
	void AddEvt( std::shared_ptr<ISSZeroDegreeEvt> event );
	void AddEvt( std::shared_ptr<ISSGammaRayEvt> event );
	void AddEvt( std::shared_ptr<ISSLumeEvt> event );
	void AddEvt( std::shared_ptr<ISSCDEvt> event );

	inline unsigned int GetArrayMultiplicity(){ return array_event.size(); };
	inline unsigned int GetArrayPMultiplicity(){ return arrayp_event.size(); };
	inline unsigned int GetRecoilMultiplicity(){ return recoil_event.size(); };
	inline unsigned int GetMwpcMultiplicity(){ return mwpc_event.size(); };
	inline unsigned int GetElumMultiplicity(){ return elum_event.size(); };
	inline unsigned int GetZeroDegreeMultiplicity(){ return zd_event.size(); };
	inline unsigned int GetGammaRayMultiplicity(){ return gamma_event.size(); };
	inline unsigned int GetLumeMultiplicity(){ return lume_event.size(); };
	inline unsigned int GetCDMultiplicity(){ return cd_event.size(); };

	inline std::shared_ptr<ISSArrayEvt> GetArrayEvt( unsigned int i ){
		if( i < array_event.size() ) return std::make_shared<ISSArrayEvt>( array_event.at(i) );
		else return nullptr;
	};
	inline std::shared_ptr<ISSArrayPEvt> GetArrayPEvt( unsigned int i ){
		if( i < arrayp_event.size() ) return std::make_shared<ISSArrayPEvt>( arrayp_event.at(i) );
		else return nullptr;
	};
	inline std::shared_ptr<ISSRecoilEvt> GetRecoilEvt( unsigned int i ){
		if( i < recoil_event.size() ) return std::make_shared<ISSRecoilEvt>( recoil_event.at(i) );
		else return nullptr;
	};
	inline std::shared_ptr<ISSMwpcEvt> GetMwpcEvt( unsigned int i ){
		if( i < mwpc_event.size() ) return std::make_shared<ISSMwpcEvt>( mwpc_event.at(i) );
		else return nullptr;
	};
	inline std::shared_ptr<ISSElumEvt> GetElumEvt( unsigned int i ){
		if( i < elum_event.size() ) return std::make_shared<ISSElumEvt>( elum_event.at(i) );
		else return nullptr;
	};
	inline std::shared_ptr<ISSZeroDegreeEvt> GetZeroDegreeEvt( unsigned int i ){
		if( i < zd_event.size() ) return std::make_shared<ISSZeroDegreeEvt>( zd_event.at(i) );
		else return nullptr;
	};
	inline std::shared_ptr<ISSGammaRayEvt> GetGammaRayEvt( unsigned int i ){
		if( i < gamma_event.size() ) return std::make_shared<ISSGammaRayEvt>( gamma_event.at(i) );
		else return nullptr;
	};
	inline std::shared_ptr<ISSLumeEvt> GetLumeEvt( unsigned int i ){
		if( i < lume_event.size() ) return std::make_shared<ISSLumeEvt>( lume_event.at(i) );
		else return nullptr;
	};
	inline std::shared_ptr<ISSCDEvt> GetCDEvt( unsigned int i ){
		if( i < cd_event.size() ) return std::make_shared<ISSCDEvt>( cd_event.at(i) );
		else return nullptr;
	};

	void ClearEvt();

	// ISOLDE timestamping
	inline void SetEBIS( double t ){ ebis = t; return; };
	inline void SetT1( double t ){ t1 = t; return; };
	inline void SetSC( double t ){ sc = t; return; };
	inline void SetLaserStatus( bool l ){ laser = l; return; };

	inline double GetEBIS(){ return ebis; };
	inline double GetT1(){ return t1; };
	inline double GetSC(){ return sc; };
	inline bool GetLaserStatus(){ return laser; };

	double GetTime();


protected:

	// variables for timestamping
	double ebis;		///< absolute EBIS pulse time
	double t1;			///< absolute proton pulse time
	double sc;			///< absolute SuperCycle pulse time
	bool laser;			///< laser status, true = ON, false = OFF

	std::vector<ISSArrayEvt> array_event;
	std::vector<ISSArrayPEvt> arrayp_event;
	std::vector<ISSRecoilEvt> recoil_event;
	std::vector<ISSMwpcEvt> mwpc_event;
	std::vector<ISSElumEvt> elum_event;
	std::vector<ISSZeroDegreeEvt> zd_event;
	std::vector<ISSGammaRayEvt> gamma_event;
	std::vector<ISSLumeEvt> lume_event;
	std::vector<ISSCDEvt> cd_event;

	ClassDef( ISSEvts, 9 )

};

#endif

