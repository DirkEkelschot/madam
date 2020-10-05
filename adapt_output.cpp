#include "adapt_output.h"

using namespace std;


void OutputMesh_MMG(MMG5_pMesh mmgMesh)
{
    std::ofstream myfile;
    myfile.open("mmgMesh_latest.dat");
    myfile << "TITLE=\"new_volume.tec\"" << std::endl;
    myfile <<"VARIABLES = \"X\", \"Y\", \"Z\"" << std::endl;
    myfile <<"ZONE N = " << mmgMesh->np << ", E = " << mmgMesh->ne << ", DATAPACKING = POINT, ZONETYPE = FETETRAHEDRON" << std::endl;

    for(int i=0;i<mmgMesh->np;i++)
    {
        myfile << mmgMesh->point[i+1].c[0] << " " <<mmgMesh->point[i+1].c[1] << " " << mmgMesh->point[i+1].c[2] <<  std::endl;
    }
    for(int i=1;i<=mmgMesh->ne;i++)
    {
        myfile << mmgMesh->tetra[i].v[0] << " " << mmgMesh->tetra[i].v[1] << " " << mmgMesh->tetra[i].v[2] << " " << mmgMesh->tetra[i].v[3] << std::endl;
    }
    myfile.close();
}






void OutputBoundaryID_MMG(MMG5_pMesh mmgMesh, std::map<int,std::vector<int> > ref2bface, int bndID)
{
    std::cout << "Writing boundary from mmgMesh "<< bndID << " to boundary_" << bndID<<  ".dat" << std::endl;
    map< int, int > Loc2GlobBound;
    map< int, Vert> BC_verts;

    std::vector<int> bfaceIDs = ref2bface[bndID];
    
    int n_bc_faces  =  bfaceIDs.size();
    int* Loc        = new int[n_bc_faces*3];
    int cnt = 0;
    Vert V;
    int tel = 0;
    std::cout << bndID << " " << bfaceIDs.size() << std::endl;
    for(int j=0;j<bfaceIDs.size();j++)
    {
        int fid = bfaceIDs[j];
        for(int k=0;k<3;k++)
        {
            int val = mmgMesh->tria[fid].v[k];
            if ( Loc2GlobBound.find( val ) != Loc2GlobBound.end() )
            {
                Loc[tel*3+k]=Loc2GlobBound[val];
            }
            else
            {
                Loc2GlobBound[val] = cnt;
                Loc[tel*3+k]=cnt;
                V.x = mmgMesh->point[val].c[0];
                V.y = mmgMesh->point[val].c[1];
                V.z = mmgMesh->point[val].c[2];
                BC_verts[cnt] = V;
                cnt++;
            }
        }
        tel++;
    }
//
    ofstream myfile;

    string filename = "boundary_mmg_" + std::to_string(bndID) + ".dat";

    myfile.open(filename);
    myfile << "TITLE=\"boundary.tec\"" << std::endl;
    myfile <<"VARIABLES = \"X\", \"Y\", \"Z\"" << std::endl;
    //ZONE N = 64, E = 48, DATAPACKING = POINT, ZONETYPE = FEQUADRILATERAL
    myfile <<"ZONE N = " << BC_verts.size() << ", E = " << n_bc_faces << ", DATAPACKING = POINT, ZONETYPE = FETRIANGLE" << std::endl;
    for(int i=0;i<BC_verts.size();i++)
    {
      myfile << BC_verts[(i)].x << "   " << BC_verts[(i)].y << "   " << BC_verts[(i)].z << std::endl;
    }

    for(int i=0;i<n_bc_faces;i++)
    {
      myfile << Loc[i*3+0]+1 << "    " << Loc[i*3+1]+1 << "   " << Loc[i*3+2]+1 << std::endl;
    }
    myfile.close();

    delete[] Loc;
    
}


void OutputBoundaryID(Partition* Pa, Mesh_Topology* meshTopo, US3D* us3d, int bndID)
{
    std::cout << "Writing boundary "<< bndID << " to boundary_" << bndID<<  ".dat" << std::endl;
    map< int, int > Loc2GlobBound;
    map< int, Vert> BC_verts;

    std::map<int,int> gV2lV               = Pa->getGlobalVert2LocalVert();
    std::vector<Vert> locVerts            = Pa->getLocalVerts();
    std::map<int,int> face2ref            = meshTopo->getFace2Ref();
    std::map<int,std::vector<int> > ref2face            = meshTopo->getRef2Face();
    std::map<int,std::vector<int> > ref2vert            = meshTopo->getRef2Vert();
    int* bnd_map = us3d->bnd_map;

    std::vector<int> bfaceIDs = ref2face[bndID];
    
    int n_bc_faces  =  bfaceIDs.size();
    int* Loc        = new int[n_bc_faces*4];
    int cnt = 0;
    Vert V;
    int tel = 0;
    std::cout << bfaceIDs[0] << " " << bfaceIDs[bfaceIDs.size()-1] << " " << us3d->nBnd << " " << n_bc_faces << std::endl;
    
    cout << "\nMin Element = "
    << *min_element(bfaceIDs.begin(), bfaceIDs.end());
    cout << "\nMax Element = "
    << *max_element(bfaceIDs.begin(), bfaceIDs.end());
    
    for(int j=0;j<bfaceIDs.size();j++)
    {
        int fid = bfaceIDs[j];
        for(int k=0;k<4;k++)
        {
            int val = us3d->ifn->getVal(fid,k);
            int lvid = gV2lV[val];
            if ( Loc2GlobBound.find( val ) != Loc2GlobBound.end() )
            {
                Loc[tel*4+k]=Loc2GlobBound[val];
            }
            else
            {
                Loc2GlobBound[val] = cnt;
                Loc[tel*4+k]=cnt;
                V.x = locVerts[lvid].x;
                V.y = locVerts[lvid].y;
                V.z = locVerts[lvid].z;
                BC_verts[cnt] = V;
                cnt++;
            }
        }
        tel++;
    }
//
    ofstream myfile;

    string filename = "boundary_" + std::to_string(bndID) + ".dat";

    myfile.open(filename);
    myfile << "TITLE=\"boundary.tec\"" << std::endl;
    myfile <<"VARIABLES = \"X\", \"Y\", \"Z\"" << std::endl;
    //ZONE N = 64, E = 48, DATAPACKING = POINT, ZONETYPE = FEQUADRILATERAL
    myfile <<"ZONE N = " << BC_verts.size() << ", E = " << n_bc_faces << ", DATAPACKING = POINT, ZONETYPE = FEQUADRILATERAL" << std::endl;
    for(int i=0;i<BC_verts.size();i++)
    {
      myfile << BC_verts[(i)].x << "   " << BC_verts[(i)].y << "   " << BC_verts[(i)].z << std::endl;
    }

    for(int i=0;i<n_bc_faces;i++)
    {
      myfile << Loc[i*4+0]+1 << "    " << Loc[i*4+1]+1 << "   " << Loc[i*4+2]+1 << "  " << Loc[i*4+3]+1 << std::endl;
    }
    myfile.close();

    delete[] Loc;
    
}


void PlotBoundaryData(Array<char>* znames, Array<int>* zdefs)
{
    int nrow = zdefs->getNrow();
    int ncol = znames->getNcol();

    std::cout << "printing boundary data..." << nrow << " " << zdefs->getNcol() << std::endl;
    for(int i=0;i<nrow;i++)
    {
        for(int j=0;j<ncol;j++)
        {
            //std::cout << " (" << i << "," << j << ") ";
            std::cout << znames->getVal(i,j) << "";
        }
        std::cout << " :: ";
        for(int j=0;j<zdefs->getNcol();j++)
        {
            std::cout << zdefs->getVal(i,j) << " ";
        }
        std::cout << std::endl;
    }
    
}




void OutputPartition(Partition* part, ParArray<int>* ien, Array<double>* H,  MPI_Comm comm)
{
    
    int size;
    MPI_Comm_size(comm, &size);
    // Get the rank of the process
    int rank;
    MPI_Comm_rank(comm, &rank);
    std::vector<std::vector<int> > loc_elem2verts_loc = part->getLocalElem2LocalVert();
    std::map<int,int> globV2locV = part->getGlobalVert2LocalVert();
    int nloc = ien->getNrow();
    int ncol = ien->getNcol();
    string filename = "quantity_rank_" + std::to_string(rank) + ".dat";
    ofstream myfile;
    myfile.open(filename);
    myfile << "TITLE=\"volume_part_"  + std::to_string(rank) +  ".tec\"" << std::endl;
    //myfile <<"VARIABLES = \"X\", \"Y\", \"Z\",  \"drhodx\",  \"drhody\",  \"drhodz\"" << std::endl;
    myfile <<"VARIABLES = \"X\", \"Y\", \"Z\"" << std::endl;
    std::vector<Vert> LVerts =  part->getLocalVerts();



        
    Array<int>* ien_local= new Array<int>(nloc,ncol); 
    std::set<int> v_used;
    std::vector<int> LocalVerticesID;
    for(int i=0;i<nloc;i++)
    {
        for(int j=0;j<ncol;j++)
        {
	    int g_v_id = ien->getVal(i,j);
	    int lv_id = globV2locV[g_v_id];
            ien_local->setVal(i,j,lv_id);
            if(v_used.find(lv_id)==v_used.end())
            {
                v_used.insert(lv_id);
                LocalVerticesID.push_back(lv_id);
            }
        }
    }
    
    
    delete ien_local;



    
    int nvert = LocalVerticesID.size();
    myfile <<"ZONE N = " << nvert << ", E = " << nloc << ", DATAPACKING = POINT, ZONETYPE = FEBRICK" << std::endl;
    //std::cout << rank << " number of nodes -> " << nvert << " " << H->getNrow() << std::endl;
    /*for(int i=0;i<nvert;i++)
    {
       myfile << LVerts[LocalVerticesID[i]].x << "   " << LVerts[LocalVerticesID[i]].y << "   " << LVerts[LocalVerticesID[i]].z << "	" << H->getVal(LocalVerticesID[i],0) << "	"<< H->getVal(LocalVerticesID[i],1) << "	" << H->getVal(LocalVerticesID[i],2) <<  std::endl;
    }*/
    for(int i=0;i<nvert;i++)
    {   
       myfile << LVerts[LocalVerticesID[i]].x << "   " << LVerts[LocalVerticesID[i]].y << "   " << LVerts[LocalVerticesID[i]].z <<  std::endl;
    }
    for(int i=0;i<nloc;i++)
    {
       myfile << loc_elem2verts_loc[i][0]+1 << "  " <<
                 loc_elem2verts_loc[i][1]+1 << "  " <<
                 loc_elem2verts_loc[i][2]+1 << "  " <<
                 loc_elem2verts_loc[i][3]+1 << "  " <<
                 loc_elem2verts_loc[i][4]+1 << "  " <<
                 loc_elem2verts_loc[i][5]+1 << "  " <<
                 loc_elem2verts_loc[i][6]+1 << "  " <<
                 loc_elem2verts_loc[i][7]+1 << std::endl;
    }
    
    
    myfile.close();
}


void OutputBLElements(Partition* part, std::vector<int> elements,  MPI_Comm comm)
{
    
    int size;
    MPI_Comm_size(comm, &size);
    // Get the rank of the process
    int rank;
    MPI_Comm_rank(comm, &rank);
    std::vector<Vert> LVerts                          =  part->getLocalVerts();
    std::vector<std::vector<int> > loc_elem2verts_loc =  part->getLocalElem2LocalVert();
    std::map<int,int> gE2lE                           =  part->getGlobalElement2LocalElement();
    std::set<int> v_used;
    std::vector<int> LocalVerticesID;
    int vid,gEl,lEl;
    std::set<int> vert_used;
    std::vector<int> vert_plot;
    std::map<int,int> gv2lv;
    Array<int>* ien_bl = new Array<int>(elements.size(),8);
    int lvid = 0;
    for(int i=0;i<elements.size();i++)
    {
        gEl = elements[i];
        lEl = gE2lE[gEl];
        
        for(int j=0;j<8;j++)
        {
            vid = loc_elem2verts_loc[lEl][j];
        
            if(gEl==0)
            {
                std::cout << " printen " << rank << " " << vid << " " << gEl << std::endl;
            }
            if(vert_used.find(vid)==vert_used.end())
            {
                vert_used.insert(vid);
                vert_plot.push_back(vid);
                gv2lv[vid] = lvid;
                ien_bl->setVal(i,j,lvid);
                lvid++;
            }
            else
            {
                ien_bl->setVal(i,j,gv2lv[vid]);
            }
        }
    }
    
    string filename = "BL_Mesh_" + std::to_string(rank) + ".dat";
    ofstream myfile;
    myfile.open(filename);
    myfile << "TITLE=\"BL_part_"  + std::to_string(rank) +  ".tec\"" << std::endl;
    myfile <<"VARIABLES = \"X\", \"Y\", \"Z\"" << std::endl;
    myfile <<"ZONE N = " << vert_plot.size() << ", E = " << elements.size() << ", DATAPACKING = POINT, ZONETYPE = FEBRICK" << std::endl;
    
    for(int i=0;i<vert_plot.size();i++)
    {
        myfile << LVerts[vert_plot[i]].x << "   " << LVerts[vert_plot[i]].y << "   " << LVerts[vert_plot[i]].z << std::endl;
        if(LVerts[vert_plot[i]].y>0.08)
        {
            std::cout << LVerts[vert_plot[i]].x << " " << LVerts[vert_plot[i]].y << " " << LVerts[vert_plot[i]].z << " " << rank << " "<< " " << vert_plot[i] << " " << i << " ranho " << std::endl;
            
        }
    }
    
    for(int i=0;i<elements.size();i++)
    {
       myfile << ien_bl->getVal(i,0)+1 << "  " <<
                 ien_bl->getVal(i,1)+1 << "  " <<
                 ien_bl->getVal(i,2)+1 << "  " <<
                 ien_bl->getVal(i,3)+1 << "  " <<
                 ien_bl->getVal(i,4)+1 << "  " <<
                 ien_bl->getVal(i,5)+1 << "  " <<
                 ien_bl->getVal(i,6)+1 << "  " <<
                 ien_bl->getVal(i,7)+1 << std::endl;
    }
    myfile.close();
}


void OutputCompletePartition(Partition* part, ParArray<int>* ien, Array<double>*H, MPI_Comm comm)
{
    
    int size;
    MPI_Comm_size(comm, &size);
    // Get the rank of the process
    int rank;
    MPI_Comm_rank(comm, &rank);
    std::vector<std::vector<int> > loc_elem2verts_loc = part->getLocalElem2LocalVert();
    int nloc = ien->getNrow();
    int ncol = 8;
    string filename = "quantity_rank_" + std::to_string(rank) + ".dat";
    ofstream myfile;
    myfile.open(filename);
    myfile << "TITLE=\"volume_part_"  + std::to_string(rank) +  ".tec\"" << std::endl;
    myfile <<"VARIABLES = \"X\", \"Y\", \"Z\",  \"drhodx\",  \"drhody\",  \"drhodz\"" << std::endl;
    //myfile <<"VARIABLES = \"X\", \"Y\", \"Z\"" << std::endl;
    std::vector<Vert> LVerts =  part->getLocalVerts();
    int nvert = LVerts.size();
    myfile <<"ZONE N = " << nvert << ", E = " << nloc << ", DATAPACKING = POINT, ZONETYPE = FEBRICK" << std::endl;
    //std::cout << rank << " number of nodes -> " << nvert << " " << H->getNrow() << std::endl;
    for(int i=0;i<nvert;i++)
    {
       myfile << LVerts[i].x << "   " << LVerts[i].y << "   " << LVerts[i].z << " " << H->getVal(i,0) << " " << H->getVal(i,1) << " " << H->getVal(i,2) << std::endl;
    }
    
    
    for(int i=0;i<nloc;i++)
    {
       myfile << loc_elem2verts_loc[i][0]+1 << "  " <<
                 loc_elem2verts_loc[i][1]+1 << "  " <<
                 loc_elem2verts_loc[i][2]+1 << "  " <<
                 loc_elem2verts_loc[i][3]+1 << "  " <<
                 loc_elem2verts_loc[i][4]+1 << "  " <<
                 loc_elem2verts_loc[i][5]+1 << "  " <<
                 loc_elem2verts_loc[i][6]+1 << "  " <<
                 loc_elem2verts_loc[i][7]+1 << std::endl;
    }
    
    
    myfile.close();
}

//void OutputGradient(Partition* parttn, Array<double>* H, ParallelState* pstate, MPI_Comm comm)
//{
//
//    int size;
//    MPI_Comm_size(comm, &size);
//    // Get the rank of the process
//    int rank;
//    MPI_Comm_rank(comm, &rank);
//
//    int nloc = parttn->getPart()->getNrow();
//    int ncol = 8;
//
//    int loc_v_id, glob_v_id;
//    std::vector<std::vector<int> > loc_elem2verts_loc = new Array<int>(nloc,ncol);
//    int offset = pstate->getOffset(rank);
//    std::map<int,int> GlobalElement2LocalElement = parttn->getGlobalElement2LocalElement();
//    std::map<int,std::vector<int> > GlobElem2LocVerts = parttn->getGlobElem2LocVerts();
//    std::vector<int> loc_vert;
//    std::set<int> vert_used;
//    std::vector<int> vert_plot;
//    for(int i=0;i<nloc;i++)
//    {
//        int g_el_id = i+offset;
//        loc_vert = GlobElem2LocVerts[g_el_id];
//        for(int j=0;j<8;j++)
//        {
//            loc_v_id = loc_elem2verts_loc[i][j];
//            if(vert_used.find(loc_v_id)==vert_used.end())
//            {
//                vert_used.insert(loc_v_id);
//                vert_plot.push_back(loc_v_id);
//            }
//
//        }
//        loc_vert.erase(loc_vert.begin(),loc_vert.end());
//    }
//
//
//    string filename = "quantity_rank_" + std::to_string(rank) + ".dat";
//    ofstream myfile;
//    myfile.open(filename);
//    myfile << "TITLE=\"volume_part_"  + std::to_string(rank) +  ".tec\"" << std::endl;
//    myfile <<"VARIABLES = \"X\", \"Y\", \"Z\", \"rho\", \"drhox\", \"drhoy\", \"drhoz\"" << std::endl;
//    std::vector<Vert> LVerts =  parttn->getLocalVerts();
//    int nvert = vert_plot.size();
//    myfile <<"ZONE N = " << nvert << ", E = " << nloc << ", DATAPACKING = POINT, ZONETYPE = FEBRICK" << std::endl;
//    Array<double>* U0 = parttn->getUvert();
//    //std::cout << rank << " number of nodes -> " << nvert << " " << H->getNrow() << std::endl;
//    for(int i=0;i<vert_plot.size();i++)
//    {
//       myfile << LVerts[vert_plot[i]].x << "   " << LVerts[vert_plot[i]].y << "   " << LVerts[vert_plot[i]].z << "   " << U0->getVal(vert_plot[i],0) << " " << H->getVal(vert_plot[i],0) << " " << H->getVal(vert_plot[i],1) << " " << H->getVal(vert_plot[i],2) << std::endl;
//    }
//
//
//    for(int i=0;i<nloc;i++)
//    {
//       myfile << loc_elem2verts_loc[i][0]+1 << "  " <<
//                 loc_elem2verts_loc[i][1]+1 << "  " <<
//                 loc_elem2verts_loc[i][2]+1 << "  " <<
//                 loc_elem2verts_loc[i][3]+1 << "  " <<
//                 loc_elem2verts_loc[i][4]+1 << "  " <<
//                 loc_elem2verts_loc[i][5]+1 << "  " <<
//                 loc_elem2verts_loc[i][6]+1 << "  " <<
//                 loc_elem2verts_loc[i][7]+1 << std::endl;
//    }
//
//
//    myfile.close();
//}



void OutputZone(Partition* part, Array<double>* H, MPI_Comm comm)
{
    
    int size;
    MPI_Comm_size(comm, &size);
    // Get the rank of the process
    int rank;
    MPI_Comm_rank(comm, &rank);
    std::vector<std::vector<int> > loc_elem2verts_loc = part->getLocalElem2LocalVert();
    int nloc = loc_elem2verts_loc.size();
    int ncol = 8;
    string filename = "quantity_rank_" + std::to_string(rank) + ".dat";
    ofstream myfile;
    myfile.open(filename);
    myfile << "TITLE=\"volume_part_"  + std::to_string(rank) +  ".tec\"" << std::endl;
    myfile <<"VARIABLES = \"X\", \"Y\", \"Z\", \"rho\", \"drhox\", \"drhoy\", \"drhoz\"" << std::endl;
    std::vector<Vert> LVerts =  part->getLocalVerts();
    int nvert = LVerts.size();
    myfile <<"ZONE N = " << nvert << ", E = " << nloc << ", DATAPACKING = POINT, ZONETYPE = FEBRICK" << std::endl;
    Array<double>* U0 = part->getUvert();
    //std::cout << rank << " number of nodes -> " << nvert << " " << H->getNrow() << std::endl;
    for(int i=0;i<nvert;i++)
    {
       myfile << LVerts[i].x << "   " << LVerts[i].y << "   " << LVerts[i].z << "   " << U0->getVal(i,0) << " " << H->getVal(i,0) << " " << H->getVal(i,1) << " " << H->getVal(i,2) << std::endl;
    }
    
    
    for(int i=0;i<nloc;i++)
    {
       myfile << loc_elem2verts_loc[i][0]+1 << "  " <<
                 loc_elem2verts_loc[i][1]+1 << "  " <<
                 loc_elem2verts_loc[i][2]+1 << "  " <<
                 loc_elem2verts_loc[i][3]+1 << "  " <<
                 loc_elem2verts_loc[i][4]+1 << "  " <<
                 loc_elem2verts_loc[i][5]+1 << "  " <<
                 loc_elem2verts_loc[i][6]+1 << "  " <<
                 loc_elem2verts_loc[i][7]+1 << std::endl;
    }
    
    
    myfile.close();
}

void OutputQuantityPartition(Partition_old* pa, Array<double>* Quan, MPI_Comm comm)
{
    int size;
    MPI_Comm_size(comm, &size);
    // Get the rank of the process
    int rank;
    MPI_Comm_rank(comm, &rank);
    
    int nrow = pa->ien->getNrow();
    int ncol = pa->ien->getNcol();
    int nloc = nrow;
    
    int gid;
    int lid;
    Vert V;
    
    //set<int> gid_set;
    std::map<int,Vert> vert_out;
    std::map<int,double> quan_out;
    int v=0;int u=0;int el=0;
    int* l_vert_id = new int[nrow*(ncol-1)];
    map< int, int > gid_set;

    double Q;
    for(int i=0;i<nloc;i++)
    {
        Q = Quan->getVal(i,0);
        for(int j=0;j<ncol-1;j++)
        {
            gid = pa->ien->getVal(i,j+1)-1;
            lid = pa->glob2loc_Vmap[gid];
            
            if ( gid_set.find( gid ) != gid_set.end() )
            {
                l_vert_id[el*(ncol-1)+j]=gid_set[gid];
            }
            else
            {
                l_vert_id[el*(ncol-1)+j]=v;
                
                V.x = pa->Verts->getVal(lid,0);
                V.y = pa->Verts->getVal(lid,1);
                V.z = pa->Verts->getVal(lid,2);
                
                vert_out[u] = V;
                quan_out[u] = Q;
                u++;
            }
            v++;
        }
        el++;
    }
    
    string filename = "quantity_rank_" + std::to_string(rank) + ".dat";
    ofstream myfile;
    myfile.open(filename);
    myfile << "TITLE=\"volume_part_"  + std::to_string(rank) +  ".tec\"" << std::endl;
    myfile <<"VARIABLES = \"X\", \"Y\", \"Z\", \"dJ\"" << std::endl;
    myfile <<"ZONE N = " << vert_out.size() << ", E = " << nrow << ", DATAPACKING = POINT, ZONETYPE = FEBRICK" << std::endl;
    
    std::cout << "number of nodes -> " << vert_out.size() << std::endl;
    
    for(int i=0;i<vert_out.size();i++)
    {
       myfile << vert_out[(i)].x << "   " << vert_out[(i)].y << "   " << vert_out[(i)].z << "   " << quan_out[(i)] << std::endl;
    }

    for(int i=0;i<nrow;i++)
    {
       myfile << l_vert_id[i*8+0]+1 << "  " <<
                 l_vert_id[i*8+1]+1 << "  " <<
                 l_vert_id[i*8+2]+1 << "  " <<
                 l_vert_id[i*8+3]+1 << "  " <<
                 l_vert_id[i*8+4]+1 << "  " <<
                 l_vert_id[i*8+5]+1 << "  " <<
                 l_vert_id[i*8+6]+1 << "  " <<
                 l_vert_id[i*8+7]+1 << std::endl;
    }
    
    
    myfile.close();
    delete[] l_vert_id;

}

/*
void OutputPartionVolumes(ParArray<int>* ien, Array<double>* xcn_on_root, MPI_Comm comm)
{
    int size;
    MPI_Comm_size(comm, &size);
    // Get the rank of the process
    int rank;
    MPI_Comm_rank(comm, &rank);
    Partition_old* pv = CollectVerticesPerRank(ien,xcn_on_root,comm);
    
    int nrow = ien->getNrow();
    int ncol = ien->getNcol();
    
    int gid;
    int lid;
    Vert V;
    
    //set<int> gid_set;
    std::map<int,Vert> vert_out;
    int v=0;int u=0;int el=0;
    int* l_vert_id = new int[nrow*(ncol-1)];
    map< int, int > gid_set;
    for(int i=0;i<nrow;i++)
    {
        for(int j=0;j<ncol-1;j++)
        {
            gid = ien->getVal(i,j+1)-1;
            lid = pv->glob2loc_Vmap[gid];
            
            if ( gid_set.find( gid ) != gid_set.end() )
            {
                l_vert_id[el*(ncol-1)+j]=gid_set[gid];
            }
            else
            {
                l_vert_id[el*(ncol-1)+j]=v;
                
                V.x = pv->Verts->getVal(lid,0);
                V.y = pv->Verts->getVal(lid,1);
                V.z = pv->Verts->getVal(lid,2);
                
                vert_out[u]=V;
                
                u++;
            }
            v++;
        }
        el++;
    }
    
    string filename = "volume_per_rank_" + std::to_string(rank) + ".dat";
    ofstream myfile;
    myfile.open(filename);
    myfile << "TITLE=\"volume_part_"  + std::to_string(rank) +  ".tec\"" << std::endl;
    myfile <<"VARIABLES = \"X\", \"Y\", \"Z\"" << std::endl;
    myfile <<"ZONE N = " << vert_out.size() << ", E = " << nrow << ", DATAPACKING = POINT, ZONETYPE = FEBRICK" << std::endl;
    
    std::cout << "number of nodes -> " << vert_out.size() << std::endl;
    
    for(int i=0;i<vert_out.size();i++)
    {
       myfile << vert_out[(i)].x << "   " << vert_out[(i)].y << "   " << vert_out[(i)].z << std::endl;
    }

    for(int i=0;i<nrow;i++)
    {
       myfile << l_vert_id[i*8+0]+1 << "  " <<
                 l_vert_id[i*8+1]+1 << "  " <<
                 l_vert_id[i*8+2]+1 << "  " <<
                 l_vert_id[i*8+3]+1 << "  " <<
                 l_vert_id[i*8+4]+1 << "  " <<
                 l_vert_id[i*8+5]+1 << "  " <<
                 l_vert_id[i*8+6]+1 << "  " <<
                 l_vert_id[i*8+7]+1 << std::endl;
    }
    myfile.close();
    delete[] l_vert_id;
}
*/

/*
void OutputPartitionFaces()
{
    MPI_Comm comm = MPI_COMM_WORLD;
    MPI_Info info = MPI_INFO_NULL;
    
    int size;
    MPI_Comm_size(comm, &size);
    // Get the rank of the process
    int rank;
    MPI_Comm_rank(comm, &rank);
    const char* fn_conn="grids/adept/conn.h5";
    const char* fn_grid="grids/adept/grid.h5";
    ParArray<int>* ief = ReadDataSetFromFileInParallel<int>(fn_conn,"ief",comm,info);

    std::vector<int> partfaces = GetAdjacencyForUS3D_V4(ief, comm);
    
    if(rank == 0)
    {
        Array<double>* xcn = ReadDataSetFromFile<double>(fn_grid,"xcn");
        Array<int>* ifn = ReadDataSetFromFile<int>(fn_grid,"ifn");
        int n_bc_faces = partfaces.size();
        Vert V;
        int* Loc = new int[n_bc_faces*4];
        map< int, int > Loc2GlobBound;
        map< int, Vert> P_verts;
        int cnt = 0;
        int tel = 0;
        int teller = 0;

        for(int j=0;j<partfaces.size();j++)
        {
            int face = partfaces[j];
            for(int k=0;k<4;k++)
            {
                int val = ifn->getVal(face,k+1);

                if ( Loc2GlobBound.find( val ) != Loc2GlobBound.end() )
                {
                    Loc[tel*4+k]=Loc2GlobBound[val];
                }
                else
                {
                    Loc2GlobBound[val] = cnt;
                    Loc[tel*4+k]=cnt;
                    V.x = xcn->getVal(val-1,0);
                    V.y = xcn->getVal(val-1,1);
                    V.z = xcn->getVal(val-1,2);
                    
                    P_verts[cnt] = V;
                    
                    cnt++;
                }
                
                teller=teller+1;
            }
            tel++;
        }
        
        ofstream myfile;
        
        string filename = "partfaces_" + std::to_string(rank) + ".dat";
        
        myfile.open(filename);
        myfile << "TITLE=\"partitionfaces.tec\"" << std::endl;
        myfile <<"VARIABLES = \"X\", \"Y\", \"Z\"" << std::endl;
        //ZONE N = 64, E = 48, DATAPACKING = POINT, ZONETYPE = FEQUADRILATERAL
        myfile <<"ZONE N = " << P_verts.size() << ", E = " << n_bc_faces << ", DATAPACKING = POINT, ZONETYPE = FEQUADRILATERAL" << std::endl;
        
        std::cout << "number of boundary nodes -> " << P_verts.size() << std::endl;
        
        for(int i=0;i<P_verts.size();i++)
        {
           myfile << P_verts[(i)].x << "   " << P_verts[(i)].y << "   " << P_verts[(i)].z << std::endl;
        }
        
        for(int i=0;i<n_bc_faces;i++)
        {
           myfile << Loc[i*4+0]+1 << "  "
                  << Loc[i*4+1]+1 << "  "
                  << Loc[i*4+2]+1 << "  "
                  << Loc[i*4+3]+1 << std::endl;
        }
        myfile.close();
        
        delete[] Loc;
        delete ifn;
        delete xcn;
     }
}
*/


void WriteBoundaryDataInSerial3(Array<double>* xcn)
{
    string filename = "boundary_nodes.dat";
    ofstream myfile;
    myfile.open(filename);
    
    
    for(int i=0;i<3097156;i++)
    {
        myfile << xcn->getVal(i,0) << " " << xcn->getVal(i,1) << " " << xcn->getVal(i,2) << std::endl;
    }
    myfile.close();
}


void WriteBoundaryDataInSerial(Array<double>* xcn, Array<int>* zdefs, Array<int>* ifn, double* detJ_verts, double* vol_verts, double* Jnorm_verts)
{
    for(int bc=3;bc<zdefs->getNrow();bc++)
    {
        map< int, int > Loc2GlobBound;
        map< int, Vert> BC_verts;
        map< int, double> J_verts;
        map< int, double> V_verts;
        map< int, double> Jno_verts;
        int n_bc_faces = zdefs->getVal(bc,4)-zdefs->getVal(bc,3);
        int* Loc = new int[n_bc_faces*4];
        
        int b_start = zdefs->getVal(bc,3)-1;
        int b_end   = zdefs->getVal(bc,4)-1;
        int cnt = 0;
        Vert V;
        int tel = 0;
        for(int j=b_start;j<b_end;j++)
        {
            for(int k=0;k<4;k++)
            {
                int val = ifn->getVal(j,k+1);
                
                if ( Loc2GlobBound.find( val ) != Loc2GlobBound.end() )
                {
                    Loc[tel*4+k]=Loc2GlobBound[val];
                }
                else
                {
                    Loc2GlobBound[val] = cnt;
                    Loc[tel*4+k]=cnt;
                    V.x = xcn->getVal(val-1,0);
                    V.y = xcn->getVal(val-1,1);
                    V.z = xcn->getVal(val-1,2);
                    BC_verts[cnt] = V;
                    //std::cout << detJ_verts->getVal(val-1,0)*1.0e08 << std::endl;
                    
                    J_verts[cnt]  = detJ_verts[val-1];
                    V_verts[cnt]  = vol_verts[val-1];
                    Jno_verts[cnt]  = Jnorm_verts[val-1];
                    cnt++;
                }
            }
            tel++;
        }
        
        ofstream myfile;
        
        string filename = "boundary_" + std::to_string(bc) + ".dat";
        
        myfile.open(filename);
        myfile << "TITLE=\"boundary.tec\"" << std::endl;
        myfile <<"VARIABLES = \"X\", \"Y\", \"Z\", \"dJ\", \"Vol\", \"dJ/Vol\"" << std::endl;
        //ZONE N = 64, E = 48, DATAPACKING = POINT, ZONETYPE = FEQUADRILATERAL
        myfile <<"ZONE N = " << BC_verts.size() << ", E = " << n_bc_faces << ", DATAPACKING = POINT, ZONETYPE = FEQUADRILATERAL" << std::endl;
        for(int i=0;i<BC_verts.size();i++)
        {
           myfile << BC_verts[(i)].x << "   " << BC_verts[(i)].y << "   " << BC_verts[(i)].z << "   " << J_verts[(i)] << "   " << V_verts[(i)] << "   " << Jno_verts[(i)] << std::endl;
        }
        
        for(int i=0;i<n_bc_faces;i++)
        {
           myfile << Loc[i*4+0]+1 << "    " << Loc[i*4+1]+1 << "   " << Loc[i*4+2]+1 << "  " << Loc[i*4+3]+1 << std::endl;
        }
        myfile.close();
        
        delete[] Loc;
    }
}



void WriteBoundaryDataInSerial2(Array<double>* xcn, Array<int>* zdefs, Array<int>* ifn, double* vol_verts)
{
    for(int bc=3;bc<zdefs->getNrow();bc++)
    {
        map< int, int > Loc2GlobBound;
        map< int, Vert> BC_verts;
        map< int, double> J_verts;
        map< int, double> V_verts;
        map< int, double> Jno_verts;
        int n_bc_faces = zdefs->getVal(bc,4)-zdefs->getVal(bc,3);
        int* Loc = new int[n_bc_faces*4];
        
        int b_start = zdefs->getVal(bc,3)-1;
        int b_end   = zdefs->getVal(bc,4)-1;
        int cnt = 0;
        Vert V;
        int tel = 0;
        //int* pltJ = new int[n_bc_faces*4];
        int teller = 0;
        for(int j=b_start;j<b_end;j++)
        {
            for(int k=0;k<4;k++)
            {
                int val = ifn->getVal(j,k+1);
                //pltJ[teller] = val;
                //std::cout << val << " ";
                if ( Loc2GlobBound.find( val ) != Loc2GlobBound.end() )
                {
                    Loc[tel*4+k]=Loc2GlobBound[val];
                }
                else
                {
                    Loc2GlobBound[val] = cnt;
                    Loc[tel*4+k]=cnt;
                    V.x = xcn->getVal(val-1,0);
                    V.y = xcn->getVal(val-1,1);
                    V.z = xcn->getVal(val-1,2);
                    BC_verts[cnt] = V;
                    //std::cout << detJ_verts->getVal(val-1,0)*1.0e08 << std::endl;
                    
                    V_verts[cnt]   = vol_verts[val-1];
                    cnt++;
                }
                
                teller=teller+1;
            }
            //std::cout << std::endl;
            tel++;
        }
        //cout << "\nlargest id = " << largest(pltJ,n_bc_faces*4) << std::endl;
        
        ofstream myfile;
        
        string filename = "boundary_" + std::to_string(bc) + ".dat";
        
        myfile.open(filename);
        myfile << "TITLE=\"boundary.tec\"" << std::endl;
        myfile <<"VARIABLES = \"X\", \"Y\", \"Z\", \"Vol\"" << std::endl;
        //ZONE N = 64, E = 48, DATAPACKING = POINT, ZONETYPE = FEQUADRILATERAL
        myfile <<"ZONE N = " << BC_verts.size() << ", E = " << n_bc_faces << ", DATAPACKING = POINT, ZONETYPE = FEQUADRILATERAL" << std::endl;
        
        std::cout << "number of boundary nodes -> " << BC_verts.size() << std::endl;
        
        for(int i=0;i<BC_verts.size();i++)
        {
           myfile << BC_verts[(i)].x << "   " << BC_verts[(i)].y << "   " << BC_verts[(i)].z << "   " << V_verts[i] << std::endl;
        }
        
        for(int i=0;i<n_bc_faces;i++)
        {
           myfile << Loc[i*4+0]+1 << "    " << Loc[i*4+1]+1 << "   " << Loc[i*4+2]+1 << "  " << Loc[i*4+3]+1 << std::endl;
        }
        myfile.close();
        
        delete[] Loc;
    }
}
