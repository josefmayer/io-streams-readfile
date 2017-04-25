#include <string>
#include <cstdio>
#include <cerrno>
#include <fstream>
#include <time.h>
#include <sstream>

// method C: C API: FILE
std::string get_file_contents_1(const char *filename)
{
	std::FILE *fp = std::fopen(filename, "rb");
	if (fp)
	{
		std::string contents;
		std::fseek(fp, 0, SEEK_END);
		contents.resize(std::ftell(fp));
		std::rewind(fp);
		std::fread(&contents[0], 1, contents.size(), fp);
		std::fclose(fp);
		return(contents);
	}
	throw(errno);
}

// method C++: C++ ifstram
std::string get_file_contents_2(const char *filename)
{
	std::ifstream in(filename, std::ios::in | std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}

// method iterator: ifstreambuf_iterator
std::string get_file_contents_3(const char *filename)
{
	std::ifstream in(filename, std::ios::in | std::ios::binary);
	if (in)
	{
		return(std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>()));
	}
	throw(errno);
}

// method assign: using std::string.assign() 
std::string get_file_contents_4(const char *filename)
{
	std::ifstream in(filename, std::ios::in | std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.reserve(in.tellg());
		in.seekg(0, std::ios::beg);
		contents.assign((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
		in.close();
		return(contents);
	}
	throw(errno);
}

// method copy: using std::copy()
std::string get_file_contents_5(const char *filename)
{
	std::ifstream in(filename, std::ios::in | std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.reserve(in.tellg());
		in.seekg(0, std::ios::beg);
		std::copy((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>(), std::back_inserter(contents));
		in.close();
		return(contents);
	}
	throw(errno);
}

// method rdbuf: copy stream on internal buffers with operator <<
std::string get_file_contents_6(const char *filename)
{
	std::ifstream in(filename, std::ios::in | std::ios::binary);
	if (in)
	{
		std::ostringstream contents;
		contents << in.rdbuf();
		in.close();
		return(contents.str());
	}
	throw(errno);
}

int main() {
	char filename[] = "text";

	struct timespec requestStart, requestEnd;
	double accum;
	int loops = 100;


	printf("size of textfile: 100 kB\n");
	printf("%s %i\n", "number of reads: ", loops);
	printf("%s\n", "time in ms: ");


	clock_gettime(CLOCK_REALTIME, &requestStart);
	for (int i = 0; i < loops; i++) {
		get_file_contents_1(filename);
	}
	clock_gettime(CLOCK_REALTIME, &requestEnd);
	accum = (requestEnd.tv_sec - requestStart.tv_sec) * 1000 +
		(requestEnd.tv_nsec - requestStart.tv_nsec) / 1000000;
	printf("%s %lf\n", "C:       ", accum);

	clock_gettime(CLOCK_REALTIME, &requestStart);
	for (int i = 0; i < loops; i++) {
		get_file_contents_2(filename);
	}
	clock_gettime(CLOCK_REALTIME, &requestEnd);
	accum = (requestEnd.tv_sec - requestStart.tv_sec) * 1000 +
		(requestEnd.tv_nsec - requestStart.tv_nsec) / 1000000;
	printf("%s %lf\n", "C++      ", accum);

	clock_gettime(CLOCK_REALTIME, &requestStart);
	for (int i = 0; i < loops; i++) {
		get_file_contents_3(filename);
	}
	clock_gettime(CLOCK_REALTIME, &requestEnd);
	accum = (requestEnd.tv_sec - requestStart.tv_sec) * 1000 +
		(requestEnd.tv_nsec - requestStart.tv_nsec) / 1000000;
	printf("%s %lf\n", "Iterator:", accum);

	clock_gettime(CLOCK_REALTIME, &requestStart);
	for (int i = 0; i < loops; i++) {
		get_file_contents_4(filename);
	}
	clock_gettime(CLOCK_REALTIME, &requestEnd);
	accum = (requestEnd.tv_sec - requestStart.tv_sec) * 1000 +
		(requestEnd.tv_nsec - requestStart.tv_nsec) / 1000000;
	printf("%s %lf\n", "Assign:  ", accum);

	clock_gettime(CLOCK_REALTIME, &requestStart);
	for (int i = 0; i < loops; i++) {
		get_file_contents_5(filename);
	}
	clock_gettime(CLOCK_REALTIME, &requestEnd);
	accum = (requestEnd.tv_sec - requestStart.tv_sec) * 1000 +
		(requestEnd.tv_nsec - requestStart.tv_nsec) / 1000000;
	printf("%s %lf\n", "Copy:    ", accum);

	clock_gettime(CLOCK_REALTIME, &requestStart);
	for (int i = 0; i < loops; i++) {
		get_file_contents_6(filename);
	}
	clock_gettime(CLOCK_REALTIME, &requestEnd);
	accum = (requestEnd.tv_sec - requestStart.tv_sec) * 1000 +
		(requestEnd.tv_nsec - requestStart.tv_nsec) / 1000000;
	printf("%s %lf\n", "Rdbuf:   ", accum);

}