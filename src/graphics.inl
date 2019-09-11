// graphics.hpp


template <int N>
void drawPolygon(Graphics& graphics, const Polygon<N>& polygon, sf::Color c) {
	sf::VertexArray sfVertices(sf::LineStrip, N+1);
	for (int i = 0; i < N; ++i) {
		sfVertices[i] = sf::Vertex(game2ScreenPos(graphics, polygon.vertices[i]), c);
	}
	sfVertices[N] = sfVertices[0];
	graphics.window.draw(sfVertices);
}

inline sf::Vector2f game2ScreenPos(const Graphics& graphics, const Vector2 &v) {
	return sf::Vector2f(v[0] * graphics.ppu, graphics.videoMode.height - (v[1] * graphics.ppu));
}

inline Vector2 screen2GamePos(const Graphics& graphics, const sf::Vector2i &v) {
	return Vector2{(float)v.x, std::fabs((float)graphics.videoMode.height - (float)v.y)} / (graphics.ppu);
}



