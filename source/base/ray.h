//
//  ray.h
//  DifViewer
//
//  Created by Glenn Smith on 1/2/15.
//  Copyright (c) 2015 CouleeApps. All rights reserved.
//

#ifndef ray_h
#define ray_h

template <typename T>
class Ray {
public:
	glm::vec3 origin;
	glm::vec3 direction;

	Ray(const glm::vec3 &origin, const glm::vec3 &direction) : origin(origin), direction(direction) {};
	Ray(const T &ox, const T &oy, const T &oz, const T &dx, const T &dy, const T &dz) : origin(glm::vec3(ox, oy, oz)), direction(glm::vec3(dx, dy, dz)) {};

	bool intersects(const PlaneF &plane) const;
	bool intersects(const TriangleF &triangle) const;

	F32 distance(const TriangleF &triangle) const;
	glm::vec3 intersection(const PlaneF &plane) const;
};

#endif
