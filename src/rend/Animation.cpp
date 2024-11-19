#include "Animation.h"
#include "Model.h"
#include "Exception.h"
#include "utils.h"

#include <sys/file.h>

namespace rend
{

Animation::Animation() :
  m_frames(1)
{ }

sys::ptr<Frame> Animation::frame(int _frame)
{
  _frame = _frame % m_frames.size();

  return &m_frames[_frame];
}

sys::ptr<Transform> Frame::transform(Part& _part, bool _add)
{
  return transform(_part.name(), _add);
}

sys::ptr<Transform> Frame::transform(const sys::string& _part, bool _add)
{
  sys::ptr<Transform> rtn(transform(_part));

  if(rtn) return rtn;
  if(!_add) return NULL;

  rtn = m_transforms.push_back(Transform());
  rtn->part = _part;
  rtn->scale = vec3(1, 1, 1);

  return rtn;
}

sys::ptr<Transform> Frame::transform(const Part& _part)
{
  return transform(_part.name());
}

sys::ptr<Transform> Frame::transform(const sys::string& _part)
{
  for(size_t ti = 0; ti < m_transforms.size(); ++ti)
  {
    if(m_transforms[ti].part == _part)
    {
      return &m_transforms[ti];
    }
  }

  return NULL;
}

static Transform interpolate(const Transform& _a, const Transform& _b, float _weight)
{
  Transform rtn;

  rtn.translation = _a.translation + (_b.translation - _a.translation) * _weight;
  rtn.rotation = _a.rotation + (_b.rotation - _a.rotation) * _weight;
  rtn.scale = _a.scale + (_b.scale - _a.scale) * _weight;

  return rtn;
}

void Animation::apply(Transform& _transform, float _frame)
{
  sys::string part(_transform.part);

  if(!part.length())
  {
    throw Exception("No part specified");
  }

  Transform empty;
  empty.scale = vec3(1, 1, 1);
  sys::ptr<rend::Frame> fa = frame(_frame);
  sys::ptr<rend::Frame> fb = frame(_frame + 1);
  sys::ptr<rend::Transform> ta = fa->transform(part);
  sys::ptr<rend::Transform> tb = fb->transform(part);

  if(!ta) ta = &empty;
  if(!tb) tb = &empty;

  float weight = _frame - (int)_frame;
  _transform = interpolate(*ta, *tb, weight);
  _transform.part = part;
}

/*
vec3 Animation::rotation(const Part& _part, float _frame)
{
  Transform empty;
  empty.scale = vec3(1, 1, 1);
  sys::Ptr<rend::Frame> fa = frame(_frame);
  sys::Ptr<rend::Frame> fb = frame(_frame + 1);
  sys::Ptr<rend::Transform> ta = fa->transform(_part);
  sys::Ptr<rend::Transform> tb = fb->transform(_part);

  if(!ta) ta = &empty;
  if(!tb) tb = &empty;

  float weight = _frame - (int)_frame;
  Transform merged(interpolate(*ta, *tb, weight));

  //printf("%f %f %f %f\n", merged.rotation.x, merged.rotation.y, merged.rotation.z, weight);

  return merged.rotation;
}

mat4 Animation::model(const Part& _part, float _frame)
{
  Transform empty;
  empty.scale = vec3(1, 1, 1);
  sys::Ptr<rend::Frame> fa = frame(_frame);
  sys::Ptr<rend::Frame> fb = frame(_frame + 1);
  sys::Ptr<rend::Transform> ta = fa->transform(_part);
  sys::Ptr<rend::Transform> tb = fb->transform(_part);

  if(!ta) ta = &empty;
  if(!tb) tb = &empty;

  float weight = _frame - (int)_frame;
  Transform merged(interpolate(*ta, *tb, weight));

  rend::mat4 model(1.0f);
  model = rend::translate(model, _part.offset);
  model = rend::translate(model, merged.translation);

  model = rend::rotate(model, rend::radians(merged.rotation.y), rend::vec3(0, 1, 0));
  model = rend::rotate(model, rend::radians(merged.rotation.x), rend::vec3(1, 0, 0));
  model = rend::rotate(model, rend::radians(merged.rotation.z), rend::vec3(0, 0, 1));

  //model *= toMat4(t->rot);

  model = rend::scale(model, merged.scale);

  model = rend::translate(model, -_part.offset);

  return model;
}
*/

size_t Animation::frames() const
{
  return m_frames.size();
}

void Animation::duplicate_frame(int _frame)
{
  Frame f = m_frames[_frame];
  m_frames.insert(f, _frame);
}

void Animation::remove_frame(int _frame)
{
  sys::vector<Frame>::iterator it = m_frames.begin() + _frame;
  m_frames.erase(it);
}

void Animation::reset_frame(int _frame)
{
  m_frames[_frame] = Frame();
}

void Animation::clear()
{
  m_frames.clear();
  m_frames.push_back(Frame());
}

mat4 Transform::model(const Part& _part) const
{
  rend::mat4 rtn(1.0f);

  if(!part.length()) return rtn;

  // Rotate around parts center rather than model
  rtn = rend::translate(rtn, _part.center());

  rtn = rend::translate(rtn, translation);

  rtn = rend::rotate(rtn, rend::radians(rotation.y), rend::vec3(0, 1, 0));
  rtn = rend::rotate(rtn, rend::radians(rotation.x), rend::vec3(1, 0, 0));
  rtn = rend::rotate(rtn, rend::radians(rotation.z), rend::vec3(0, 0, 1));

  rtn = rend::scale(rtn, scale);

  rtn = rend::translate(rtn, -_part.center());

  return rtn;
}

void Animation::load(const sys::string& _path)
{
  m_frames.clear();

  sys::file f(_path);
  sys::string line;
  sys::vector<sys::string> tokens;
  sys::vector<sys::string> parts;

  sys::ptr<Frame> frame;

  while(f.read_line(line))
  {
    if(line.length() < 1) continue;

    split_string_whitespace(line, tokens);

    if(tokens[0] == "p" && tokens.size() >= 2)
    {
      bool found = false;

      for(sys::vector<sys::string>::iterator pit = parts.begin();
        pit != parts.end(); ++pit)
      {
        if(sys::string(*pit) == tokens[1])
        {
          found = true;
        }
      }

      if(!found)
      {
        parts.push_back(tokens[1]);
      }
    }
    else if(tokens[0] == "f")
    {
      m_frames.push_back(Frame());
      frame = &m_frames[m_frames.size() - 1];
    }
    else if(tokens[0] == "t" && tokens.size() >= 11)
    {
      sys::string partname(parts[atoi(tokens[1].unsafe_raw())]);
      sys::ptr<Transform> t(frame->transform(partname, true));

      t->translation = vec3(
        atof(tokens[2].unsafe_raw()),
        atof(tokens[3].unsafe_raw()),
        atof(tokens[4].unsafe_raw())
      );

      t->rotation = vec3(
        atof(tokens[5].unsafe_raw()),
        atof(tokens[6].unsafe_raw()),
        atof(tokens[7].unsafe_raw())
      );

      t->scale = vec3(
        atof(tokens[8].unsafe_raw()),
        atof(tokens[9].unsafe_raw()),
        atof(tokens[10].unsafe_raw())
      );
    }
  }
}

sys::string Animation::serialize()
{
  sys::vector<sys::string> parts;
  sys::string frames;

  for(sys::vector<Frame>::iterator fit = m_frames.begin();
    fit != m_frames.end(); ++fit)
  {
    frames += "f\n";

    for(sys::vector<Transform>::iterator tit = fit->transforms_begin();
      tit != fit->transforms_end(); ++tit)
    {
      sys::string partname(tit->part);
      int partindex = -1;

      for(size_t i = 0; i < parts.size(); ++i)
      {
        if(parts[i] == partname)
        {
          partindex = i;

          break;
        }
      }

      if(partindex == -1)
      {
        parts.push_back(partname);
        partindex = parts.size() - 1;
      }

      sys::string line("t ");
      line += sys::string(partindex) + " ";

      line += sys::string(tit->translation.x) + " ";
      line += sys::string(tit->translation.y) + " ";
      line += sys::string(tit->translation.z) + " ";

      line += sys::string(tit->rotation.x) + " ";
      line += sys::string(tit->rotation.y) + " ";
      line += sys::string(tit->rotation.z) + " ";

      line += sys::string(tit->scale.x) + " ";
      line += sys::string(tit->scale.y) + " ";
      line += sys::string(tit->scale.z) + "\n";

      frames += line;
    }
  }

  sys::string rtn;

  for(sys::vector<sys::string>::iterator pit = parts.begin();
    pit != parts.end(); ++pit)
  {
    rtn += "p " + *pit + "\n";
  }

  rtn += "\n";
  rtn += frames;

  return rtn;
}

sys::vector<Transform>::iterator Frame::transforms_begin()
{
  return m_transforms.begin();
}

sys::vector<Transform>::iterator Frame::transforms_end()
{
  return m_transforms.end();
}

}

