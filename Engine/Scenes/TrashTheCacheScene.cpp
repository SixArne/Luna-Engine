#include "pch.h"
#include "TrashTheCacheScene.h"
#include <imgui.h>
#include <implot.h>

Engine::TrashTheCacheScene::TrashTheCacheScene(const std::string& scenename)
	:Scene(scenename)
{


	m_Iterations = {
		1,
		2,
		4,
		8,
		16,
		32,
		64,
		128,
		256,
		512,
		1024
	};

	m_Ex1Results.resize(m_Iterations.size());
	m_Ex2Results.resize(m_Iterations.size());
	m_Ex2AltResults.resize(m_Iterations.size());
}

void Engine::TrashTheCacheScene::OnImGui()
{

	// I have no idea why this isn't displaying anything when pressing the button.
	// I'll know points will be deducted by this but I kinda like this library,
	// so ill ask the community on how to resolve this. Sheers!

	ImGui::Begin("Exercise 1");
	if (ImGui::Button("Run benchmark", ImVec2(100, 20)))
	{
		BenchMarkExercise1();
		m_name = "CHANGED";
	}

	if (ImPlot::BeginPlot(m_name.c_str()))
	{

		ImPlot::SetupAxes("Allocation size", "Time");
		ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.5f);
		ImPlot::PlotLine("Speed", m_Iterations.data(), m_Ex1Results.data(), (int)m_Iterations.size());
		ImPlot::EndPlot();
	}

	ImGui::End();

	ImGui::Begin("Exercise 2");
	if (ImGui::Button("Run benchmark", ImVec2(100, 20)))
	{
		BenchMarkExercise2();
		m_name = "CHANGED";
	}

	if (ImPlot::BeginPlot(m_name.c_str()))
	{

		ImPlot::SetupAxes("Allocation size", "Time");
		ImPlot::PlotLine("Speed", m_Iterations.data(), m_Ex2Results.data(), (int)m_Iterations.size());
		ImPlot::EndPlot();
	}

	ImGui::End();

	ImGui::Begin("Exercise 2alt");
	if (ImGui::Button("Run benchmark", ImVec2(100, 20)))
	{
		BenchMarkExercise2Alt();
		m_name = "CHANGED";
	}

	if (ImPlot::BeginPlot(m_name.c_str()))
	{

		ImPlot::SetupAxes("Allocation size", "Time");
		ImPlot::PlotLine("Speed", m_Iterations.data(), m_Ex2AltResults.data(), (int)m_Iterations.size());
		ImPlot::EndPlot();
	}

	ImGui::End();
}

void Engine::TrashTheCacheScene::BenchMarkExercise1()
{
	// clear old list
	m_Ex1Results.clear();

	auto integerBuffer = std::vector<int>((int)std::pow(2, 10));

	std::map<int, std::vector<long long>> sampleData{};

	size_t sampleCount{ 10 };
	for (size_t sampleIndex{}; sampleIndex < sampleCount; ++sampleIndex)
	{
		for (size_t step{ 1 }; step <= 1024; step *= 2)
		{
			const auto start = std::chrono::high_resolution_clock::now();

			for (size_t j{}; j < integerBuffer.size(); j += step)
			{
				integerBuffer[j] *= 2;
			}
			const auto end = std::chrono::high_resolution_clock::now();
			const auto total = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

			if (sampleData.count((int)step) == 0)
			{
				sampleData[(int)step] = std::vector<long long>{};
			}
			sampleData[(int)step].push_back(total);
		}
	}

	for (auto data : sampleData)
	{
		data.second.erase(std::max_element(begin(data.second), end(data.second)));
		data.second.erase(std::min_element(begin(data.second), end(data.second)));

		auto total{ std::accumulate(begin(data.second), end(data.second),0LL, std::plus<long long>()) };
		double average{ total / (double)sampleCount };

		m_Ex1Results.push_back(average);
	}
}

void Engine::TrashTheCacheScene::BenchMarkExercise2()
{
	m_Ex2Results.clear();

	std::vector<GameObject3D*> arr((int)std::pow(2, 26));

	for (size_t i{}; i < arr.size(); ++i)
	{
		arr[i] = new GameObject3D{ (int)i };
	}
	std::map<int, std::vector<long long>> sampleData{};
	size_t sampleCount{ 10 };
	for (size_t sampleIndex{}; sampleIndex < sampleCount; ++sampleIndex)
	{
		for (size_t stepsize{ 1 }; stepsize <= 1024; stepsize *= 2)
		{
			auto start = std::chrono::high_resolution_clock::now();

			for (size_t i{}; i < arr.size(); i += stepsize)
			{
				arr[i]->ID *= 2;
			}

			auto end = std::chrono::high_resolution_clock::now();
			auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();


			if (sampleData.count((int)stepsize) == 0)
			{
				sampleData[(int)stepsize] = std::vector<long long>{};
			}
			sampleData[(int)stepsize].push_back(elapsed);
		}
	}

	for (auto data : sampleData)
	{
		data.second.erase(std::max_element(begin(data.second), end(data.second)));
		data.second.erase(std::min_element(begin(data.second), end(data.second)));

		auto total{ std::accumulate(begin(data.second), end(data.second),0LL, std::plus<long long>()) };
		double average{ total / (double)sampleCount };

		m_Ex2Results.push_back(average);
	}
}

void Engine::TrashTheCacheScene::BenchMarkExercise2Alt()
{
	m_Ex2AltResults.clear();

	std::vector<GameObject3DAlt*> arr((int)std::pow(2, 26));

	for (size_t i{}; i < arr.size(); ++i)
	{
		arr[i] = new GameObject3DAlt{ (int)i };
	}
	std::map<int, std::vector<long long>> sampleData{};
	size_t sampleCount{ 10 };
	for (size_t sampleIndex{}; sampleIndex < sampleCount; ++sampleIndex)
	{
		for (size_t stepsize{ 1 }; stepsize <= 1024; stepsize *= 2)
		{
			auto start = std::chrono::high_resolution_clock::now();

			for (size_t i{}; i < arr.size(); i += stepsize)
			{
				arr[i]->ID *= 2;
			}

			auto end = std::chrono::high_resolution_clock::now();
			auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();


			if (sampleData.count((int)stepsize) == 0)
			{
				sampleData[(int)stepsize] = std::vector<long long>{};
			}
			sampleData[(int)stepsize].push_back(elapsed);
		}
	}

	for (auto data : sampleData)
	{
		data.second.erase(std::max_element(begin(data.second), end(data.second)));
		data.second.erase(std::min_element(begin(data.second), end(data.second)));

		auto total{ std::accumulate(begin(data.second), end(data.second),0LL, std::plus<long long>()) };
		double average{ total / (double)sampleCount };

		m_Ex2AltResults.push_back(average);
	}
}
