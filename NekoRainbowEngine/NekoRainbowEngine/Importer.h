#ifndef _IMPORTER_H_
#define _IMPORTER_H_

class Importer
{
public:
	Importer() {};
	~Importer() {};

	virtual bool Init() { return true; };
	virtual bool CleanUp() { return true; }

private:

};

#endif // !_IMPORTER_H_

