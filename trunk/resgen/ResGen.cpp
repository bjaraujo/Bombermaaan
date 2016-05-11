/************************************************************************************

    Copyright (C) 2008 Markus Drescher

    This file is part of Bombermaaan.

    Bombermaaan is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Bombermaaan is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Bombermaaan.  If not, see <http://www.gnu.org/licenses/>.

************************************************************************************/

/// ResGen.cpp
/// This file generates the resource dll source code
/// It has to be executed in order to generate the resource library
/// Its execution is only required in Linux

#include "ResGen.h"

// static two dimensional array containing resource informations
// first dimension: 0 images, 1 sounds
// second dimension: struct array
static SRES *resources[NUM_TYPES];

/**
 * @brief   determine the number of resources (BITMAPs and SOUNDs)
 * @param   rcfile          reference to a open rc file
 * @param   hfile           reference to a open header file
 * @param   rcBitmapCount   reference to the Bitmap counter (rc file)
 * @param   rcSoundCount    reference to the Sound counter  (rc file)
 * @param   hBitmapCount    reference to the Bitmap counter (h file)
 * @param   hSoundCount     reference to the Sound counter  (h file)
 */
void determineResourceCount(ifstream& rcfile, ifstream& hfile,
    int& rcBitmapCount, int& rcSoundCount,
    int& hBitmapCount, int& hSoundCount)
{
    string Line;
    
    // count resources (i.e. BITMAPs and SOUNDs) in rc file
    while (rcfile.good()) {
        getline(rcfile, Line);
        
        if ((int)Line.find("DISCARDABLE") != -1)
        {
            if ((int)Line.find("BITMAP") != -1)
                rcBitmapCount++;
            else if ((int)Line.find("SOUND") != -1)
                rcSoundCount++;
        }
    }
    
    // count resources (i.e. BITMAPs and SOUNDs) in header file
    while (hfile.good()) {
        getline(hfile, Line);
        
        if ((int)Line.find("#define") != -1)
        {
            if ((int)Line.find("BMP_") != -1)
                hBitmapCount++;
            else if ((int)Line.find("SND_") != -1)
                hSoundCount++;
        }
    }
}

/**
 * @brief   build array for resources
 * @param   hBitmapCount    number of bitmaps found in the first place
 * @param   hSoundCount     number of sounds found in the first place
 * @param   hfile           reference to the open header file ifstream object
 * @param   hbuf            buffer for seeking in the open header file
 * @param   bitmapNo        number of bitmaps being processed here
 * @param   soundNo         number of sounds being processed here
 * @return  true (success), false (error)
 */
bool buildResourceArrayWithHeader(int hBitmapCount, int hSoundCount,
  ifstream& hfile, filebuf *hbuf, int& bitmapNo, int& soundNo)
{
    int count;
    int pos;
    int wspace, wspace2;
    int type;
    int size;
    
    char *resname;
    char *resnumber;
    
    string Line;
    
    // valid number, now build array
    resources[TYPE_BITMAP] = new SRES[hBitmapCount];
    resources[TYPE_SOUND] = new SRES[hSoundCount];
    
    if (resources[TYPE_BITMAP] == NULL || resources[TYPE_SOUND] == NULL)
    {
        fprintf(stderr, "Error creating resource array\n");
        return false;
    }
    
    // initialise array
    for (int i = TYPE_BITMAP; i < NUM_TYPES; i++)
    {
        if (i == TYPE_BITMAP) count = hBitmapCount;
        else if (i == TYPE_SOUND) count = hSoundCount;
        else count = 0;
            
        for (int j = 0; j < count; j++)
        {
            resources[i][j].r_name = NULL;
            resources[i][j].r_path = NULL;
        }
    }
    
    // read names from header file
    hbuf->pubseekpos (0, ios::in);
    
    bitmapNo = 0;
    soundNo = 0;
    
    hfile.clear(); // clear status bits
    
    while (hfile.good()) {
        getline(hfile, Line);
        
        if ((int)Line.find("#define") != -1)
        {
            // find beginning of resource name
            pos = Line.find("BMP_");
            if (pos != -1) 
				type = TYPE_BITMAP;
            else
            {
                pos = Line.find("SND_");
                
                if (pos == -1) continue;
                type = TYPE_SOUND;
            }
            
            // find next whitespace (space/tab)
            wspace = Line.find(" ", pos);
            wspace2 = Line.find("\t", pos);
            if (wspace == -1) wspace = wspace2;
            else if (wspace2 == -1); // do nothing
            else wspace = MIN(wspace, wspace2);
            
            if (wspace == -1)
            {
                fprintf(stderr, "unable to determine resource name in line:\n %s\n", Line.c_str());
                return false;
            }
            
            // now determine resource name
            size = wspace - pos;
            resname = new char[size + 1];
            if (resname == NULL)
            {
                fprintf(stderr, "allocation error for a character array\n");
                return false;
            }
            
            // copy including terminating \0
            strncpy(resname, Line.substr(pos, size).c_str(), size+1);
            
            // now determine resource number
            pos = -1;
            
			for (int i = (int)Line.size() - 1; i > wspace; i--)
            {
                if (pos == -1 && (Line.c_str()[i] < '0' || Line.c_str()[i] > '9'))
                {
					pos = i + 1;
                    break;
                }
            }
            
            if (pos == -1)
            {
                fprintf(stderr, "unable to determine resource number for %s\n", resname);
                delete[] resname;
                return false;
            }
                        
            // now copy resource number into a buffer
			size = Line.size() - pos;
            resnumber = new char[size + 1];
            if (resnumber == NULL)
            {
                fprintf(stderr, "allocation error for a character array\n");
                return false;
            }
            
            // copy including terminating \0
            strncpy(resnumber, Line.substr(pos, size).c_str(), size + 1);
            
            // copy data to array and increment counters
            if (type == TYPE_BITMAP)
            {
                resources[type][bitmapNo].r_number = atoi(resnumber);
                resources[type][bitmapNo].r_name = resname;
                
                bitmapNo++;
            }
            else if (type == TYPE_SOUND)
            {
                resources[type][soundNo].r_number = atoi(resnumber);
                resources[type][soundNo].r_name = resname;
                
                soundNo++;
            }
            
            // resname is being deleted at the end of the application
            delete[] resnumber;
        }
    }

    return true;
}

/**
 * @brief   determine no of the element in the resource array with a certain ID
 * @param   type    resource type (currently TYPE_BITMAP or TYPE_SOUND)
 * @param   name    resource id to look for
 * @param   count   total number of elements (rcBitmapCount or rcSoundCount)
 * @return  number of the element or (-1) if it wasn't found
 */
int findResourceElement(int type, const char *name, int count)
{
    bool found = false;
    int i;
    
    for (i = 0; i < count; i++)
    {
        //cout << "comparing " << resources[type][i].r_name << " vs " << name << endl;
        if (strncmp(resources[type][i].r_name, name, strlen(name)) == 0 &&
            strlen(resources[type][i].r_name) == strlen(name))
        {
            found = true;
            break;
        }
    }
        
    if (found)
        return i;
    else
        return -1;
}

/**
 * @brief   complete the resource array with paths to the files
 * @param   rcBitmapCount   number of bitmaps found in the first place
 * @param   rcSoundCount    number of sounds found in the first place
 * @param   rcfile          reference to the open rc file
 * @param   rcbuf           buffer used for seeking in the open rc file
 * @param   bitmapNo        number of bitmaps treated in this function
 * @param   soundNo         number of sounds treated in this function
 * @return  true (success), false (error)
 */
bool completeResourceArrayWithRC(int rcBitmapCount, int rcSoundCount,
    ifstream& rcfile, filebuf *rcbuf, int& bitmapNo, int& soundNo)
{
    string Line;
    
    char *resname;
    char *respath;
    
    int count;
    
    int el;
    int pos;
    int wspace, wspace2;
    int type;
    
    int size;

    // read names from resource file
    rcbuf->pubseekpos (0, ios::in);
    
    bitmapNo = 0;
    soundNo = 0;
    
    rcfile.clear(); // clear status bits
    
    while (rcfile.good()) {
        getline(rcfile, Line);
        
        pos = Line.find("DISCARDABLE");
        if (pos != -1)
        {
            if ((int)Line.find("BITMAP") != -1)
            {
                type = TYPE_BITMAP;
                count = rcBitmapCount;
            }
            else if ((int)Line.find("SOUND") != -1)
            {
                type = TYPE_SOUND;
                count = rcSoundCount;
            }
            else
            {
                // no valid type
                continue;
            }
            
            // determine proper name
            wspace = Line.find(" ");
            wspace2 = Line.find("\t");
            
            if (wspace == -1) wspace = wspace2;
            else if (wspace2 == -1); // do nothing
            else wspace = MIN(wspace, wspace2);
            
            size = wspace;
            resname = new char[size + 1];
            if (resname == NULL) return false;
            
            strncpy(resname, Line.c_str(), size);
            resname[size] = '\0';
            
            el = findResourceElement(type, resname, count);
            
            delete[] resname;
            
            // do not add a path if this element was not found
            if (el == -1) continue;
                        
            // element found, now determine path (which is included in "")
            pos = Line.find("\"", pos);
            
            if (pos == -1) return false;
            
            wspace = Line.rfind("\"");
            
            if (wspace == -1 || wspace <= pos) return false;
            
            // got a valid path.
            pos++;
            
            size = wspace - pos;
            respath = new char[size + 1];
            if (respath == NULL) return false;
            
            // copy including terminating \0
            for (int i = 0, j = 0; j < size; i++)
            {
                respath[i] = Line.c_str()[pos+j];
                
                // a backslash needs \\ because of the escape sequence
                if (respath[i] == '\\')
                {
                    respath[i] = '/';
                    j++;
                }
                j++;
                
                if (j == size) respath[i+1] = '\0';
            }
            
            resources[type][el].r_path = respath;
            
            // respath is deleted at the deletion of the resources array
            
            if (type == TYPE_BITMAP)
                bitmapNo++;
            else if (type == TYPE_SOUND)
                soundNo++;
        }
    }
        
    return true;
}

/**
 * @brief   generate cpp file for file inclusion
 * @param   hBitmapCount    number of bitmaps found in the first place
 * @param   hSoundCount     number of sounds found in the first place
 * @param   realBitmapCount number of correctly parsed bitmaps
 * @param   realSoundCount  number of correctly parsed sounds
 * @return  true (success) or false (error)
 */
bool generateSourceCode(int hBitmapCount, int hSoundCount,
                        int realBitmapCount, int realSoundCount)
{
    bool error = false;
    size_t *len[NUM_TYPES];
    size_t offset;
    size_t size;
    int count;
    unsigned char buf[BUFSIZE];
    
	string resPath;
    
    const char header[] = "#include <stdlib.h>\n\n" \
        "typedef struct _SRESOURCE {\n" \
        "\tint r_id;\n" \
        "\tunsigned char *r_data;\n" \
        "\tunsigned long r_len;\n" \
        "} SRESOURCE;\n\n";
    
    const char footer[] = "unsigned char *getResource(int type, int id, unsigned long *len)\n" \
        "{\n\tint count;\n\tint found = 0;\n\tint i;\n\n" \
        "\tif (type == 0) count = RES0_COUNT;\n" \
        "\telse if (type == 1) count = RES1_COUNT;\n\telse return NULL;\n\n" \
        "\tfor (i = 0; i < count; i++)\n\t{\n" \
	    "\t\tif (resources[type][i].r_id == id)\n\t\t{\n" \
        "\t\t\tfound = 1;\n\t\t\tbreak;\n\t\t}\n\t}\n\n" \
        "\tif (!found) return NULL;\n\n" \
        "\t*len = resources[type][i].r_len;\n\n" \
        "\treturn resources[type][i].r_data;\n}\n";

    if (realBitmapCount <= 0 && realSoundCount <= 0) return false;
    
    ifstream res;
    ofstream out;
    
    out.open(RESDATAC, ios_base::out);
    if (!out.is_open()) return false;
    
    // file header
    out.write(header, strlen(header));
              
    // number of resources
    out << "#define RES0_COUNT " << realBitmapCount << endl;
    out << "#define RES1_COUNT " << realSoundCount << endl << endl;
    
    // initialise len array
    len[TYPE_BITMAP] = new size_t[hBitmapCount];
    len[TYPE_SOUND] = new size_t[hSoundCount];
    
    if (len[TYPE_BITMAP] == NULL || len[TYPE_SOUND] == NULL)
    {
        cerr << "Could not generate source code. Out of memory." << endl;
        return false;
    }
    
    // data and sound
    for (int type = TYPE_BITMAP; type < NUM_TYPES && !error; type++)
    {
        if (type == TYPE_BITMAP)
            count = hBitmapCount;
        else
            count = hSoundCount;
        
        for (int i = 0, j = 0; i < count; i++)
        {
            // i is the counter for the resources array
            // j is the counter for the true number of resources
            
            // ignore empty paths
            if (resources[type][i].r_path == NULL){
                cerr << "WARNING: " << resources[type][i].r_name << " ("
                     << resources[type][i].r_number
                     << ") will not be included. Empty path." << endl;
                continue;
            }
            
			resPath = "";
			resPath.append(RES_DIR);

#ifdef WIN32
			resPath.append("\\");
#else
			resPath.append("/");
#endif

			resPath.append(resources[type][i].r_path);
            
			res.open(resPath.c_str(), ios_base::in | ios_base::binary);
            
            if (!res.is_open())
            {
				cerr << "Could not open file " << resources[type][i].r_path << endl;
                error = true;
                break;
            }
            
            // find out file size
            res.seekg(0,ifstream::end);
            len[type][i] = res.tellg();
            res.seekg(0);
            offset = 0;
            
            // write beginning (with length)
            out << "static unsigned char res" << type << "_" << j;
            out << "[" << len[type][i] << "] = {";
            
            // now read data
            while (res.good())
            {
                res.read((char *)buf, BUFSIZE);
                
                size = res.gcount();
                for (unsigned int j = 0; j < size; j++)
                {
                    out << "0x" << hex << ((unsigned int)buf[j]);
                    if (j != size - 1)
                    {
                        out << ",";
                        if (j % 16 == 10) out << endl;
                    }
                    
                }
                
                offset += size;
                if (offset != len[type][i]) out << ",";
            }
                
            out << "};" << dec << endl;
            
            res.close();
            j++;
        }
        
        out << endl;
    }
    
    if (error)
    {
        delete[] len[TYPE_BITMAP];
        delete[] len[TYPE_SOUND];
        
        cerr << "Could not generate source code." << endl;
        
        out.close();
        return false;
    }

    out << "static SRESOURCE resources0[RES0_COUNT] = {";
    
    for (int i = 0, j = 0; i < hBitmapCount; i++)
    {
        // j is again the counter of valid resources
        if (resources[TYPE_BITMAP][i].r_path == NULL) continue;
        
        out << "{" << resources[TYPE_BITMAP][i].r_number << ",res0_" << j;
        out << "," << len[TYPE_BITMAP][i] << "}";
        
        if (i != hBitmapCount - 1)
        {
            out << ", ";
            if (j % 4 == 2) out << endl;
        }
        j++;
    }
    
    out << "};" << endl << "static SRESOURCE resources1[RES1_COUNT] = {";
    
    for (int i = 0, j = 0; i < hSoundCount; i++)
    {
        // j is again the counter of valid resources
        if (resources[TYPE_SOUND][i].r_path == NULL) continue;
        
        out << "{" << resources[TYPE_SOUND][i].r_number << ",res1_" << j;
        out << "," << len[TYPE_SOUND][i] << "}";
        
        if (i != hSoundCount - 1)
        {
            out << ", ";
            if (j % 4 == 2) out << endl;
        }
        j++;
    }
    
    delete[] len[TYPE_BITMAP];
    delete[] len[TYPE_SOUND];
        
    out << "};" << endl;
    out << "static SRESOURCE *resources[2] = {resources0, resources1};\n\n";
    
    out.write(footer, strlen(footer));
              
    out.close();
    
    return true;
}

/**
 * @brief   main function
 * @param   argc    argument count
 * @param   argv    arguments
 * @return  0 (success) or -1 (failure)
 */
int main (int argc, char **argv)
{
    ifstream rcfile;
    ifstream hfile;
    
    filebuf *rcbuf;
    filebuf *hbuf;
    
    int rcBitmapCount = 0;
    int rcSoundCount = 0;
    int hBitmapCount = 0;
    int hSoundCount = 0;
    
    int bitmapNo;
    int soundNo;
    
    bool error = false;
        
    // check command line parameters
    if (argc < 2)
    {
        cout << "Wrong number of arguments!" << endl;
        return 0;
    }
    
    RES_DIR = argv[1];
    
    string resPath = RES_DIR;

#ifdef WIN32
    resPath.append("\\");
#else
    resPath.append("/");
#endif
    
    string rcPath = resPath;
    rcPath.append(RCFILE);
    
    rcfile.open(rcPath.c_str(), ios_base::in);
    
    if (!rcfile.is_open())
    {
		fprintf(stderr, "Could not open resource file %s\n", rcPath.c_str());
        return -1;
    }
    
    string hPath = resPath;
    hPath.append(HFILE);
    
    hfile.open(hPath.c_str(), ios_base::in);
    
    if (!hfile.is_open())
    {
        fprintf(stderr, "Could not open resource header file %s\n", hPath.c_str());
        return -1;
    }
    
    rcbuf = rcfile.rdbuf();
    hbuf = hfile.rdbuf();

    determineResourceCount(rcfile, hfile, rcBitmapCount, rcSoundCount, hBitmapCount, hSoundCount);
    
    // compare results
    if (hBitmapCount != rcBitmapCount)
    {
        fprintf(stderr, "WARNING: the number of bitmap resources (%d) in the "
            "resource file (" RCFILE ") differs from the number of bitmap "
            "resources (%d) in the header file (" HFILE "). Note that resource "
            "macros have to begin with BMP_ in the header file.\n",
            rcBitmapCount, hBitmapCount);
        fprintf(stderr, "Incomplete resources (i.e. its ID in the header or "
            "its filename in the rc file is missing) will not be included\n");
    }
    
    if (hSoundCount != rcSoundCount)
    {
        fprintf(stderr, "WARNING: the number of sound resources (%d) in the "
            "resource file (" RCFILE ") differs from the number of sound "
            "resources (%d) in the header file (" HFILE "). Note that resource "
            "macros have to begin with SND_ in the header file.\n",
            rcSoundCount, hSoundCount);
        fprintf(stderr, "Incomplete resources (i.e. its ID in the header or "
            "its filename in the rc file is missing) will not be included\n");
    }
    
    // now we will use hBitmapCount / hSoundCount as the valid count
    
    error = !buildResourceArrayWithHeader(hBitmapCount, hSoundCount, hfile, hbuf, bitmapNo, soundNo);
    
    // compare results
    if (hBitmapCount != bitmapNo)
    {
        fprintf(stderr, "WARNING: only %d out of %d bitmap resources were "
            "correctly parsed from the resource header (" HFILE ").\n",
            bitmapNo, hBitmapCount);

    }
    
    if (hSoundCount != soundNo)
    {
        fprintf(stderr, "WARNING: only %d out of %d sound resources were "
            "correctly parsed from the resource header (" HFILE ").\n",
            soundNo, hSoundCount);
    }
    
    if (error)
    {
        for (int i = 0; i < bitmapNo; i++)
            if (resources[TYPE_BITMAP][i].r_name != NULL)
                delete[] resources[TYPE_BITMAP][i].r_name;
        for (int i = 0; i < soundNo; i++)
            if (resources[TYPE_SOUND][i].r_name != NULL)
                delete[] resources[TYPE_SOUND][i].r_name;

        delete[] resources[TYPE_BITMAP];
        delete[] resources[TYPE_SOUND];
        
        hfile.close();
        rcfile.close();
        
        return -1;
    }
    
    error = !completeResourceArrayWithRC(hBitmapCount, hSoundCount, rcfile, rcbuf, bitmapNo, soundNo);

    // compare results
    if (hBitmapCount != bitmapNo)
    {
        fprintf(stderr, "WARNING: only %d out of %d bitmap resources were "
            "correctly parsed from the resource file (" RCFILE ").\n",
            bitmapNo, hBitmapCount);
    }
    
    if (hSoundCount != soundNo)
    {
        fprintf(stderr, "WARNING: only %d out of %d sound resources were "
            "correctly parsed from the resource file (" RCFILE ").\n",
            soundNo, hSoundCount);
    }
    
    if (error)
    {
        for (int i = 0; i < hBitmapCount; i++)
        {
            if (resources[TYPE_BITMAP][i].r_name != NULL)
                delete[] resources[TYPE_BITMAP][i].r_name;
            if (resources[TYPE_BITMAP][i].r_path != NULL)
                delete[] resources[TYPE_BITMAP][i].r_path;
        }
        for (int i = 0; i < hSoundCount; i++)
        {
            if (resources[TYPE_SOUND][i].r_name != NULL)
            delete[] resources[TYPE_SOUND][i].r_name;
            if (resources[TYPE_SOUND][i].r_path != NULL)
                delete[] resources[TYPE_SOUND][i].r_path;
        }

        delete[] resources[TYPE_BITMAP];
        delete[] resources[TYPE_SOUND];
        
        hfile.close();
        rcfile.close();
        
        return -1;
    }
    
    // we may close the files now
    hfile.close();
    rcfile.close();
    
    // now we will generate the source code, opening each resource and convert
    // its contents into a character array
    error = !generateSourceCode(hBitmapCount, hSoundCount, bitmapNo, soundNo);
    if (error)
    {
        cerr << "error creating source code.\n";
        unlink(RESDATAC);
    }
    
    // clean up
    for (int i = 0; i < hBitmapCount; i++)
    {
        delete[] resources[TYPE_BITMAP][i].r_name;
        delete[] resources[TYPE_BITMAP][i].r_path;
    }
    for (int i = 0; i < hSoundCount; i++)
    {
        delete[] resources[TYPE_SOUND][i].r_name;
        delete[] resources[TYPE_SOUND][i].r_path;
    }

    delete[] resources[TYPE_BITMAP];
    delete[] resources[TYPE_SOUND];
    
    if (!error)
    {
        cout << "Source code was successfully generated." << endl;
        return 0;
    }
    else
        return -1;
}
