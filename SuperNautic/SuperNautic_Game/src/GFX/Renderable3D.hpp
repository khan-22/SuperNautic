#pragma once

#ifndef RENDERABLE_3D_HPP
#define RENDERABLE_3D_HPP

class Renderable3D
{
public:
	virtual ~Renderable3D();

protected:
	virtual void render() = 0;

private:


};

#endif //RENDERABLE_3D_HPP
