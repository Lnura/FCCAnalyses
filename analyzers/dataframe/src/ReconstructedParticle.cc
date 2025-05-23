#include "FCCAnalyses/ReconstructedParticle.h"

// std
#include <cstdlib>
#include <stdexcept>

// ROOT
#include <ROOT/RDataFrame.hxx>
#include <ROOT/RLogger.hxx>

// EDM4hep
#include "edm4hep/EDM4hepVersion.h"

namespace FCCAnalyses{

namespace ReconstructedParticle{

/// sel_type
sel_type::sel_type(const int type) : m_type(type) {}

ROOT::VecOps::RVec<edm4hep::ReconstructedParticleData> sel_type::operator()(
    ROOT::VecOps::RVec<edm4hep::ReconstructedParticleData> in) {
  ROOT::VecOps::RVec<edm4hep::ReconstructedParticleData> result;
  result.reserve(in.size());
  for (size_t i = 0; i < in.size(); ++i) {
#if edm4hep_VERSION > EDM4HEP_VERSION(0, 10, 5)
    if (in[i].PDG == m_type) {
#else
    if (in[i].type == m_type) {
#endif
      result.emplace_back(in[i]);
    }
  }
  return result;
}

/// sel_absType
sel_absType::sel_absType(const int type) : m_type(type) {
  if (m_type < 0) {
    throw std::invalid_argument(
        "ReconstructedParticle::sel_absType: Received negative value!");
  }
}

ROOT::VecOps::RVec<edm4hep::ReconstructedParticleData> sel_absType::operator()(
    ROOT::VecOps::RVec<edm4hep::ReconstructedParticleData> in) {
  ROOT::VecOps::RVec<edm4hep::ReconstructedParticleData> result;
  result.reserve(in.size());
  for (size_t i = 0; i < in.size(); ++i) {
#if edm4hep_VERSION > EDM4HEP_VERSION(0, 10, 5)
    if (std::abs(in[i].PDG) == m_type) {
#else
    if (std::abs(in[i].type) == m_type) {
#endif
      result.emplace_back(in[i]);
    }
  }

  return result;
}

/// sel_pt
sel_pt::sel_pt(float arg_min_pt) : m_min_pt(arg_min_pt) {};
ROOT::VecOps::RVec<edm4hep::ReconstructedParticleData>  sel_pt::operator() (ROOT::VecOps::RVec<edm4hep::ReconstructedParticleData> in) {
  ROOT::VecOps::RVec<edm4hep::ReconstructedParticleData> result;
  result.reserve(in.size());
  for (size_t i = 0; i < in.size(); ++i) {
    auto & p = in[i];
    if (std::sqrt(std::pow(p.momentum.x,2) + std::pow(p.momentum.y,2)) > m_min_pt) {
      result.emplace_back(p);
    }
  }
  return result;
}

sel_eta::sel_eta(float arg_min_eta) : m_min_eta(arg_min_eta) {};
ROOT::VecOps::RVec<edm4hep::ReconstructedParticleData>  sel_eta::operator() (ROOT::VecOps::RVec<edm4hep::ReconstructedParticleData> in) {
  ROOT::VecOps::RVec<edm4hep::ReconstructedParticleData> result;
  result.reserve(in.size());
  for (size_t i = 0; i < in.size(); ++i) {
    auto & p = in[i];
    TLorentzVector tv1;
    tv1.SetXYZM(p.momentum.x, p.momentum.y, p.momentum.z, p.mass);
    if (abs(tv1.Eta()) < abs(m_min_eta)){
      result.emplace_back(p);
    }
  }
  return result;
}


sel_p::sel_p(float arg_min_p, float arg_max_p) : m_min_p(arg_min_p), m_max_p(arg_max_p)  {};
ROOT::VecOps::RVec<edm4hep::ReconstructedParticleData>  sel_p::operator() (ROOT::VecOps::RVec<edm4hep::ReconstructedParticleData> in) {
  ROOT::VecOps::RVec<edm4hep::ReconstructedParticleData> result;
  result.reserve(in.size());
  for (size_t i = 0; i < in.size(); ++i) {
    auto & p = in[i];
    float momentum = std::sqrt(   std::pow(p.momentum.x,2)
                                + std::pow(p.momentum.y,2)
                                + std::pow(p.momentum.z,2) );
    if ( momentum > m_min_p && momentum < m_max_p ) {
      result.emplace_back(p);
    }
  }
  return result;
}

sel_charge::sel_charge(int arg_charge, bool arg_abs){m_charge = arg_charge; m_abs = arg_abs;};

ROOT::VecOps::RVec<edm4hep::ReconstructedParticleData>  sel_charge::operator() (ROOT::VecOps::RVec<edm4hep::ReconstructedParticleData> in) {
  ROOT::VecOps::RVec<edm4hep::ReconstructedParticleData> result;
  result.reserve(in.size());
  for (size_t i = 0; i < in.size(); ++i) {
    auto & p = in[i];
    if ((m_abs && abs(in[i].charge)==m_charge) || (m_charge==in[i].charge) ) {
      result.emplace_back(p);
    }
  }
  return result;
}

resonanceBuilder::resonanceBuilder(float arg_resonance_mass) {m_resonance_mass = arg_resonance_mass;}
ROOT::VecOps::RVec<edm4hep::ReconstructedParticleData> resonanceBuilder::operator()(ROOT::VecOps::RVec<edm4hep::ReconstructedParticleData> legs) {
  ROOT::VecOps::RVec<edm4hep::ReconstructedParticleData> result;
  int n = legs.size();
  if (n >1) {
    ROOT::VecOps::RVec<bool> v(n);
    std::fill(v.end() - 2, v.end(), true);
    do {
      edm4hep::ReconstructedParticleData reso;
      TLorentzVector reso_lv;
      for (int i = 0; i < n; ++i) {
          if (v[i]) {
            reso.charge += legs[i].charge;
            TLorentzVector leg_lv;
            leg_lv.SetXYZM(legs[i].momentum.x, legs[i].momentum.y, legs[i].momentum.z, legs[i].mass);
            reso_lv += leg_lv;
          }
      }
      reso.momentum.x = reso_lv.Px();
      reso.momentum.y = reso_lv.Py();
      reso.momentum.z = reso_lv.Pz();
      reso.mass = reso_lv.M();
      result.emplace_back(reso);
    } while (std::next_permutation(v.begin(), v.end()));
  }
  if (result.size() > 1) {
    auto resonancesort = [&] (edm4hep::ReconstructedParticleData i ,edm4hep::ReconstructedParticleData j) { return (abs( m_resonance_mass -i.mass)<abs(m_resonance_mass-j.mass)); };
    std::sort(result.begin(), result.end(), resonancesort);
    ROOT::VecOps::RVec<edm4hep::ReconstructedParticleData>::const_iterator first = result.begin();
    ROOT::VecOps::RVec<edm4hep::ReconstructedParticleData>::const_iterator last = result.begin() + 1;
    ROOT::VecOps::RVec<edm4hep::ReconstructedParticleData> onlyBestReso(first, last);
    return onlyBestReso;
  } else {
    return result;
  }
}


recoilBuilder::recoilBuilder(float arg_sqrts) : m_sqrts(arg_sqrts) {};
ROOT::VecOps::RVec<edm4hep::ReconstructedParticleData>  recoilBuilder::operator() (ROOT::VecOps::RVec<edm4hep::ReconstructedParticleData> in) {
  ROOT::VecOps::RVec<edm4hep::ReconstructedParticleData> result;
  auto recoil_p4 = TLorentzVector(0, 0, 0, m_sqrts);
  for (auto & v1: in) {
    TLorentzVector tv1;
    tv1.SetXYZM(v1.momentum.x, v1.momentum.y, v1.momentum.z, v1.mass);
    recoil_p4 -= tv1;
  }
  auto recoil_fcc = edm4hep::ReconstructedParticleData();
  recoil_fcc.momentum.x = recoil_p4.Px();
  recoil_fcc.momentum.y = recoil_p4.Py();
  recoil_fcc.momentum.z = recoil_p4.Pz();
  recoil_fcc.mass = recoil_p4.M();
  result.push_back(recoil_fcc);
  return result;
};


sel_axis::sel_axis(bool arg_pos): m_pos(arg_pos) {};
ROOT::VecOps::RVec<edm4hep::ReconstructedParticleData> sel_axis::operator()(ROOT::VecOps::RVec<float> angle, ROOT::VecOps::RVec<edm4hep::ReconstructedParticleData> in){
  ROOT::VecOps::RVec<edm4hep::ReconstructedParticleData> result;
  for (size_t i = 0; i < angle.size(); ++i) {
    if (m_pos==1 && angle.at(i)>0.) result.push_back(in.at(i));
    if (m_pos==0 && angle.at(i)<0.) result.push_back(in.at(i));;
  }
  return result;
}


sel_tag::sel_tag(bool arg_pass): m_pass(arg_pass) {};
ROOT::VecOps::RVec<edm4hep::ReconstructedParticleData> sel_tag::operator()(ROOT::VecOps::RVec<bool> tags, ROOT::VecOps::RVec<edm4hep::ReconstructedParticleData> in){
  ROOT::VecOps::RVec<edm4hep::ReconstructedParticleData> result;
  for (size_t i = 0; i < in.size(); ++i) {
    if (m_pass) {
      if (tags.at(i)) result.push_back(in.at(i));
    }
    else {
      if (!tags.at(i)) result.push_back(in.at(i));
    }
  }
  return result;
}



// Angular separation between the particles of a collection:
//   arg_delta = 0 / 1 / 2 :   return delta_max, delta_min, delta_average

angular_separationBuilder::angular_separationBuilder( int  arg_delta) : m_delta(arg_delta) {};
float angular_separationBuilder::operator() ( ROOT::VecOps::RVec<edm4hep::ReconstructedParticleData> in) {

 float result = -9999;

 float dmax = -999;
 float dmin = 999;
 float sum = 0;
 float npairs = 0;
 for (int i=0; i < in.size(); i++) {
  if ( in.at(i).energy < 0) continue;    // "dummy" particle - cf selRP_matched_to_list
  TVector3 p1( in.at(i).momentum.x, in.at(i).momentum.y, in.at(i).momentum.z );
  for (int j=i+1; j < in.size(); j++) {
    if ( in.at(j).energy < 0) continue;   // "dummy" particle
    TVector3 p2( in.at(j).momentum.x, in.at(j).momentum.y, in.at(j).momentum.z );
    float delta_ij = fabs( p1.Angle( p2 ) );
    if ( delta_ij > dmax) dmax = delta_ij;
    if ( delta_ij < dmin) dmin = delta_ij;
    sum = sum + delta_ij;
    npairs ++;
  }
 }
 float delta_max = dmax;
 float delta_min = dmin;
 float delta_ave = sum / npairs;

 if (m_delta == 0 ) result = delta_max;
 if (m_delta == 1 ) result = delta_min;
 if (m_delta == 2 ) result = delta_ave;

 return result;
}


ROOT::VecOps::RVec<float> get_pt(ROOT::VecOps::RVec<edm4hep::ReconstructedParticleData> in){
 ROOT::VecOps::RVec<float> result;
 for (size_t i = 0; i < in.size(); ++i) {
   result.push_back(sqrt(in[i].momentum.x * in[i].momentum.x + in[i].momentum.y * in[i].momentum.y));
 }
 return result;
}

ROOT::VecOps::RVec<edm4hep::ReconstructedParticleData> merge(ROOT::VecOps::RVec<edm4hep::ReconstructedParticleData> x, ROOT::VecOps::RVec<edm4hep::ReconstructedParticleData> y) {
  //to be keept as ROOT::VecOps::RVec
  std::vector<edm4hep::ReconstructedParticleData> result;
  result.reserve(x.size() + y.size());
  result.insert( result.end(), x.begin(), x.end() );
  result.insert( result.end(), y.begin(), y.end() );
  return ROOT::VecOps::RVec(result);
}


ROOT::VecOps::RVec<edm4hep::ReconstructedParticleData> remove(
  		ROOT::VecOps::RVec<edm4hep::ReconstructedParticleData> x,
  		ROOT::VecOps::RVec<edm4hep::ReconstructedParticleData> y) {
  //to be kept as ROOT::VecOps::RVec
  std::vector<edm4hep::ReconstructedParticleData> result;
  result.reserve( x.size() );
  result.insert( result.end(), x.begin(), x.end() );
  float epsilon = 1e-8;
  for (size_t i = 0; i < y.size(); ++i) {
    float mass1 = y.at(i).mass;
    float px1 = y.at(i).momentum.x;
    float py1 = y.at(i).momentum.y;
    float pz1 = y.at(i).momentum.z;
    for(std::vector<edm4hep::ReconstructedParticleData>::iterator
          it = std::begin(result); it != std::end(result); ++it) {
      float mass2 = it->mass;
      float px2 = it->momentum.x;
      float py2 = it->momentum.y;
      float pz2 = it->momentum.z;
      if ( abs(mass1-mass2) < epsilon &&
	   abs(px1-px2) < epsilon &&
	   abs(py1-py2) < epsilon &&
	   abs(pz1-pz2) < epsilon ) {
        result.erase(it);
        break;
      }
    }
  }
  return ROOT::VecOps::RVec(result);
}




ROOT::VecOps::RVec<edm4hep::ReconstructedParticleData> get(ROOT::VecOps::RVec<int> index, ROOT::VecOps::RVec<edm4hep::ReconstructedParticleData> in){
  ROOT::VecOps::RVec<edm4hep::ReconstructedParticleData> result;
  for (size_t i = 0; i < index.size(); ++i) {
    if (index[i]>-1)
      result.push_back(in.at(index[i]));
    //else
    //  std::cout << "electron index negative " << index[i]<<std::endl;
  }
  return result;
}

TLorentzVector get_P4vis(ROOT::VecOps::RVec<edm4hep::ReconstructedParticleData> in) {
    TLorentzVector P4sum;
    for (auto & p: in) {
      TLorentzVector tlv;
      tlv.SetXYZM(p.momentum.x, p.momentum.y, p.momentum.z, p.mass);
      P4sum += tlv;
    }
    return P4sum;
  }


ROOT::VecOps::RVec<float> get_mass(ROOT::VecOps::RVec<edm4hep::ReconstructedParticleData> in) {
  ROOT::VecOps::RVec<float> result;
  for (auto & p: in) {
    result.push_back(p.mass);
  }
  return result;
}

ROOT::VecOps::RVec<float> get_eta(ROOT::VecOps::RVec<edm4hep::ReconstructedParticleData> in) {
  ROOT::VecOps::RVec<float> result;
  for (auto & p: in) {
    TLorentzVector tlv;
    tlv.SetXYZM(p.momentum.x, p.momentum.y, p.momentum.z, p.mass);
    result.push_back(tlv.Eta());
  }
  return result;
}

ROOT::VecOps::RVec<float> get_phi(ROOT::VecOps::RVec<edm4hep::ReconstructedParticleData> in) {
  ROOT::VecOps::RVec<float> result;
  for (auto & p: in) {
    TLorentzVector tlv;
    tlv.SetXYZM(p.momentum.x, p.momentum.y, p.momentum.z, p.mass);
    result.push_back(tlv.Phi());
  }
  return result;
}

ROOT::VecOps::RVec<float> get_delta_eta(ROOT::VecOps::RVec<edm4hep::ReconstructedParticleData> in) {
  ROOT::VecOps::RVec<float> result;
  for (int i = 0; i < in.size(); i++) {
    TLorentzVector tlv1;
    tlv1.SetXYZM(in[i].momentum.x, in[i].momentum.y, in[i].momentum.z, in[i].mass);
    for (auto j = i + 1; j < in.size(); j++) {
      TLorentzVector tlv2;
      tlv2.SetXYZM(in[j].momentum.x, in[j].momentum.y, in[j].momentum.z, in[j].mass);
      float delta_eta = abs(tlv1.Eta() - tlv2.Eta());
      result.push_back(delta_eta);
    }
  }
  return result;
}

ROOT::VecOps::RVec<float> get_delta_phi(ROOT::VecOps::RVec<edm4hep::ReconstructedParticleData> in) {
  ROOT::VecOps::RVec<float> result;
  for (int i = 0; i < in.size(); i++) {
    TLorentzVector tlv1;
    tlv1.SetXYZM(in[i].momentum.x, in[i].momentum.y, in[i].momentum.z, in[i].mass);
    for (auto j = i + 1; j < in.size(); j++) {
      TLorentzVector tlv2;
      tlv2.SetXYZM(in[j].momentum.x, in[j].momentum.y, in[j].momentum.z, in[j].mass);
      float delta_phi = tlv1.DeltaPhi(tlv2);
      result.push_back(delta_phi);
    }
  }
  return result;
}

ROOT::VecOps::RVec<float> get_delta_r(ROOT::VecOps::RVec<edm4hep::ReconstructedParticleData> in) {
  ROOT::VecOps::RVec<float> result;
  for (int i = 0; i < in.size(); i++) {
    TLorentzVector tlv1;
    tlv1.SetXYZM(in[i].momentum.x, in[i].momentum.y, in[i].momentum.z, in[i].mass);
    for (auto j = i + 1; j < in.size(); j++) {
      TLorentzVector tlv2;
      tlv2.SetXYZM(in[j].momentum.x, in[j].momentum.y, in[j].momentum.z, in[j].mass);
      float delta_r = tlv1.DeltaR(tlv2);
      result.push_back(delta_r);
    }
  }
  return result;
}

float get_delta_r(edm4hep::ReconstructedParticleData p1, edm4hep::ReconstructedParticleData p2) {
  TLorentzVector tlv1;
  tlv1.SetXYZM(p1.momentum.x, p1.momentum.y, p1.momentum.z, p1.mass);
  TLorentzVector tlv2;
  tlv2.SetXYZM(p2.momentum.x, p2.momentum.y, p2.momentum.z, p2.mass);
  float delta_r = tlv1.DeltaR(tlv2);
  return delta_r;
}

ROOT::VecOps::RVec<float> get_min_delta_r(ROOT::VecOps::RVec<edm4hep::ReconstructedParticleData> in) {
  ROOT::VecOps::RVec<float> result;
  float min_delta_r = 999;
  for (int i = 0; i < in.size(); i++) {
    TLorentzVector tlv1;
    tlv1.SetXYZM(in[i].momentum.x, in[i].momentum.y, in[i].momentum.z, in[i].mass);
    for (int j = i + 1; j < in.size(); j++) {
      TLorentzVector tlv2;
      tlv2.SetXYZM(in[j].momentum.x, in[j].momentum.y, in[j].momentum.z, in[j].mass);
      float delta_r = tlv1.DeltaR(tlv2);
      if (delta_r < min_delta_r) {
        min_delta_r = delta_r;
      }
    }
  }
  result.push_back(min_delta_r);
  return result;
}

ROOT::VecOps::RVec<float> get_pidx_min_delta_r(ROOT::VecOps::RVec<edm4hep::ReconstructedParticleData> in) {
  ROOT::VecOps::RVec<float> result;
  float min_delta_r = 999;
  int p1idx = -1;
  int p2idx = -1;
  for (int i = 0; i < in.size(); i++) {
    TLorentzVector tlv1;
    tlv1.SetXYZM(in[i].momentum.x, in[i].momentum.y, in[i].momentum.z, in[i].mass);
    for (int j = i + 1; j < in.size(); j++) {
      TLorentzVector tlv2;
      tlv2.SetXYZM(in[j].momentum.x, in[j].momentum.y, in[j].momentum.z, in[j].mass);
      float delta_r = tlv1.DeltaR(tlv2);
      if (delta_r < min_delta_r) {
        min_delta_r = delta_r;
        p1idx = i;
        p2idx = j;
      }
    }
  }

  if(p1idx > -1 && p2idx > -1) 
    result.emplace_back(p1idx);
    result.emplace_back(p2idx);
    // std::cout << p1idx << " " << p2idx << std::endl;
  return result;
}



ROOT::VecOps::RVec<float> get_reference_point_x(ROOT::VecOps::RVec<edm4hep::ReconstructedParticleData> in) {
  ROOT::VecOps::RVec<float> result;
  for (auto & p: in) {
    result.push_back(p.referencePoint.x);
  }
  return result;
}

ROOT::VecOps::RVec<float> get_reference_point_y(ROOT::VecOps::RVec<edm4hep::ReconstructedParticleData> in) {
  ROOT::VecOps::RVec<float> result;
  for (auto & p: in) {
    result.push_back(p.referencePoint.y);
  }
  return result;
}

ROOT::VecOps::RVec<float> get_reference_point_z(ROOT::VecOps::RVec<edm4hep::ReconstructedParticleData> in) {
  ROOT::VecOps::RVec<float> result;
  for (auto & p: in) {
    result.push_back(p.referencePoint.z);
  }
  return result;
}

ROOT::VecOps::RVec<float> get_e(ROOT::VecOps::RVec<edm4hep::ReconstructedParticleData> in) {
  ROOT::VecOps::RVec<float> result;
  for (auto & p: in) {
    result.push_back(p.energy);
  }
  return result;
}

float get_total_e(ROOT::VecOps::RVec<edm4hep::ReconstructedParticleData> in){
  float result;
  for (auto & p: in) {
    result += p.energy;
  }
  return result;
}

ROOT::VecOps::RVec<float> get_p(ROOT::VecOps::RVec<edm4hep::ReconstructedParticleData> in) {
  ROOT::VecOps::RVec<float> result;
  for (auto & p: in) {
    TLorentzVector tlv;
    tlv.SetXYZM(p.momentum.x, p.momentum.y, p.momentum.z, p.mass);
    result.push_back(tlv.P());
  }
  return result;
}

float get_diphoton_deltaR(edm4hep::ReconstructedParticleData p, edm4hep::ReconstructedParticleData diphoton1 , edm4hep::ReconstructedParticleData diphoton2) {
  ROOT::VecOps::RVec<float> result;
  TLorentzVector diphoton_tlv, p_tlv;
  diphoton_tlv.SetXYZM(diphoton1.momentum.x+diphoton2.momentum.x, diphoton1.momentum.y+diphoton2.momentum.y, diphoton1.momentum.z+diphoton2.momentum.z, diphoton1.mass+diphoton2.mass);
  p_tlv.SetXYZM(p.momentum.x, p.momentum.y, p.momentum.z, p.mass);
  float delta_r = diphoton_tlv.DeltaR(p_tlv);
  return delta_r;
}

ROOT::VecOps::RVec<float> get_px(ROOT::VecOps::RVec<edm4hep::ReconstructedParticleData> in) {
  ROOT::VecOps::RVec<float> result;
  for (auto & p: in) {
    result.push_back(p.momentum.x);
  }
  return result;
}


ROOT::VecOps::RVec<float> get_py(ROOT::VecOps::RVec<edm4hep::ReconstructedParticleData> in) {
  ROOT::VecOps::RVec<float> result;
  for (auto & p: in) {
    result.push_back(p.momentum.y);
  }
  return result;
}

ROOT::VecOps::RVec<float> get_pz(ROOT::VecOps::RVec<edm4hep::ReconstructedParticleData> in) {
  ROOT::VecOps::RVec<float> result;
  for (auto & p: in) {
    result.push_back(p.momentum.z);
  }
  return result;
}

ROOT::VecOps::RVec<float> get_charge(ROOT::VecOps::RVec<edm4hep::ReconstructedParticleData> in) {
  ROOT::VecOps::RVec<float> result;
  for (auto & p: in) {
    result.push_back(p.charge);
  }
  return result;
}

ROOT::VecOps::RVec<float> get_y(ROOT::VecOps::RVec<edm4hep::ReconstructedParticleData> in) {
  ROOT::VecOps::RVec<float> result;
  for (auto & p: in) {
    TLorentzVector tlv;
    tlv.SetXYZM(p.momentum.x, p.momentum.y, p.momentum.z, p.mass);
    result.push_back(tlv.Rapidity());
  }
  return result;
}

ROOT::VecOps::RVec<float> get_theta(ROOT::VecOps::RVec<edm4hep::ReconstructedParticleData> in) {
  ROOT::VecOps::RVec<float> result;
  for (auto & p: in) {
    TLorentzVector tlv;
    tlv.SetXYZM(p.momentum.x, p.momentum.y, p.momentum.z, p.mass);
    result.push_back(tlv.Theta());
  }
  return result;
}


ROOT::VecOps::RVec<std::vector<float>> get_prompt_photon_calorimeter_hits(ROOT::VecOps::RVec<edm4hep::ReconstructedParticleData> photon) {
  const float R = 2500.0; //calorimeter radius and length is 2500mm
  const float Zmax = 2500.0;
  ROOT::VecOps::RVec<std::vector<float>> result;
  for (auto & p : photon) {
    TLorentzVector tlv;
    tlv.SetXYZM(p.momentum.x, p.momentum.y, p.momentum.z, p.mass);
    TVector3 direction = tlv.Vect().Unit(); 
    float ux = static_cast<float>(direction.X());
    float uy = static_cast<float>(direction.Y());  
    float uz = static_cast<float>(direction.Z());  
    float t_r = R / sqrt(ux*ux + uy*uy);  
    float t_z = Zmax / fabs(uz);             
    float t_hit = std::min(t_r, t_z); 

    // float ux2 = sin(tlv.Theta())*cos(tlv.Phi());
    // float uy2 = sin(tlv.Theta())*sin(tlv.Phi());
    // float uz2 = cos(tlv.Theta());

    std::vector<float> hit_pos = {t_hit * ux, t_hit * uy, t_hit * uz};



    result.push_back(hit_pos);
  }
  return result;
}

ROOT::VecOps::RVec<std::vector<float>> get_displaced_photon_calorimeter_hits(ROOT::VecOps::RVec<edm4hep::ReconstructedParticleData> photon,float ALP_Lx, float ALP_Ly, float ALP_Lz) {
  const float R = 2500.0; //calorimeter radius and length is 2500mm
  const float Zmax = 2500.0;
  ROOT::VecOps::RVec<std::vector<float>> result;
  for (auto & p : photon) {
    TLorentzVector tlv;
    tlv.SetXYZM(p.momentum.x, p.momentum.y, p.momentum.z, p.mass);
    TVector3 direction = tlv.Vect().Unit(); 
    float ux = static_cast<float>(direction.X());
    float uy = static_cast<float>(direction.Y());  
    float uz = static_cast<float>(direction.Z());  

    ///solving quadratic equation to find intersection with calorimeter
    /// (ALP_Lx + t * ux)^2 + (ALP_Ly + t * uy)^2 = R^2 and solve for t
    /// (ux^2 + uy^2) * t^2 + 2 * (ALP_Lx * ux + ALP_Ly * uy) * t + (ALP_Lx^2 + ALP_Ly^2 - R^2) = 0
    float a = 2.0f * (ALP_Lx * ux + ALP_Ly * uy);
    float b = a * a - 4 * (ux * ux + uy * uy) * (ALP_Lx * ALP_Lx + ALP_Ly * ALP_Ly - R * R);

    float t_r = 99999999;

    if (b >= 0) {
      float t1 = (-a + sqrt(b)) / (2 * (ux * ux + uy * uy));
      float t2 = (-a - sqrt(b)) / (2 * (ux * ux + uy * uy));
      if (t1 > 0) t_r = t1;
      if (t2 > 0) t_r = std::min(t_r, t2);
    }
 
    float t_z_pos = (Zmax - ALP_Lz) / uz;
    float t_z_neg = (-Zmax - ALP_Lz) / uz;
    float t_z = 99999999;
    if (t_z_pos > 0 && fabs(ALP_Lz + t_z_pos * uz) <= Zmax) t_z = t_z_pos;
    if (t_z_neg > 0 && fabs(ALP_Lz + t_z_neg * uz) <= Zmax) t_z = std::min(t_z, t_z_neg);

    if (t_r < 0) t_r = 99999999; 
    if (t_z < 0) t_z = 99999999; 
        
    float t_hit = std::min(t_r, t_z); 

    std::vector<float> hit_pos = {ALP_Lx + t_hit * ux, ALP_Ly + t_hit * uy, ALP_Lz + t_hit * uz};
    result.push_back(hit_pos);
  }
  return result;
}




ROOT::VecOps::RVec<TLorentzVector> get_tlv(ROOT::VecOps::RVec<edm4hep::ReconstructedParticleData> in) {
  ROOT::VecOps::RVec<TLorentzVector> result;
  for (auto & p: in) {
    TLorentzVector tlv;
    tlv.SetXYZM(p.momentum.x, p.momentum.y, p.momentum.z, p.mass);
    result.push_back(tlv);
  }
  return result;
}

TLorentzVector get_tlv(ROOT::VecOps::RVec<edm4hep::ReconstructedParticleData> in, int index) {
  TLorentzVector result;
  auto & p = in[index];
  result.SetXYZM(p.momentum.x, p.momentum.y, p.momentum.z, p.mass);
  return result;
}

TLorentzVector get_tlv(edm4hep::ReconstructedParticleData in) {
  TLorentzVector result;
  result.SetXYZM(in.momentum.x, in.momentum.y, in.momentum.z, in.mass);
  return result;
}

ROOT::VecOps::RVec<int>
get_type(ROOT::VecOps::RVec<edm4hep::ReconstructedParticleData> in){
  ROOT::VecOps::RVec<int> result;
  for (auto & p: in) {
#if edm4hep_VERSION > EDM4HEP_VERSION(0, 10, 5)
    result.push_back(p.PDG);
#else
    result.push_back(p.type);
#endif
  }
  return result;
}


int get_n(ROOT::VecOps::RVec<edm4hep::ReconstructedParticleData> x) {
  int result =  x.size();
  return result;
}


ROOT::VecOps::RVec<bool> getJet_btag(ROOT::VecOps::RVec<int> index, ROOT::VecOps::RVec<edm4hep::ParticleIDData> pid, ROOT::VecOps::RVec<float> values){
  ROOT::VecOps::RVec<bool> result;
  //std::cout << "========================new event=======================" <<std::endl;
  for (size_t i = 0; i < index.size(); ++i) {
    result.push_back(values.at(pid.at(index.at(i)).parameters_begin));

    //std::cout << pid.at(index.at(i)).parameters_begin << "  ==  " << pid.at(index.at(i)).parameters_end << std::endl;
    //for (unsigned j = pid.at(index.at(i)).parameters_begin; j != pid.at(index.at(i)).parameters_end; ++j) {
    //  std::cout << " values : " << values.at(j) << std::endl;
    //}
  }
  return result;
}

int getJet_ntags(ROOT::VecOps::RVec<bool> in) {
  int result =  0;
  for (size_t i = 0; i < in.size(); ++i)
    if (in.at(i))result+=1;
  return result;
}

 float get_sum_float(ROOT::VecOps::RVec<float> in){
    float result = std::accumulate(in.begin(), in.end(), 0.0f);
    //float result = 0;
    //for (int i; i < in.size(); ++i){
    //  result += in[i];
    //}
    return result;
  }

  // get sum of int
  int get_sum_int(ROOT::VecOps::RVec<int> in){
    int result = std::accumulate(in.begin(), in.end(), 0);
    //int result = 0;
    //for (int i; i < in.size(); ++i){
    //  result += in[i];
    //}
    return result;
  }

  // get avg of floats
  float get_avg_float(ROOT::VecOps::RVec<float> in){
    //float result;
    //for (int i; i < in.size(); ++i){
    //  result += in[i];
    //}
    float result = std::accumulate(in.begin(), in.end(), 0.0f) / in.size();
    //result /= in.size();
    return result;
  }

  // get avg of ints
  int get_avg_int(ROOT::VecOps::RVec<float> in){
    //int result;
    //for (int i; i < in.size(); ++i){
    //  result += in[i];
    //}
    //result /= in.size();
    int result = std::accumulate(in.begin(), in.end(), 0.0f) / in.size();
    return result;
  }



}//end NS ReconstructedParticle

}//end NS FCCAnalyses
