/*
   Thai Language Analysis Module for etisnoop
   Thailand DAB+ Broadcasting System - Phase 4

   Comprehensive Thai language compliance checking and cultural content validation
   ETSI TS 101 756 Thai Profile (0x0E) support with Buddhist calendar integration
*/

#pragma once

#include <string>
#include <vector>
#include <map>
#include <set>
#include <memory>
#include <chrono>
#include "figs.hpp"

namespace Thai {

// Thai DAB+ compliance levels
enum class ComplianceLevel {
    COMPLIANT = 0,      // 95-100% compliance
    WARNING = 1,        // 85-94% compliance  
    NON_COMPLIANT = 2,  // 70-84% compliance
    CRITICAL = 3        // <70% compliance
};

// Thai character encoding validation result
struct CharacterValidation {
    bool valid_encoding;           // UTF-8 properly encoded
    bool dab_profile_compliant;    // ETSI TS 101 756 profile 0x0E compliant
    bool renderable;               // Can be rendered on DAB displays
    int invalid_chars;             // Number of invalid characters
    std::vector<std::string> issues; // Specific encoding issues
    double compliance_score;       // 0-100 compliance percentage
};

// Buddhist/Cultural content analysis result
struct CulturalAnalysis {
    bool has_buddhist_content;     // Contains Buddhist religious content
    bool has_royal_content;        // Contains Thai royal references  
    bool has_traditional_content;  // Contains traditional Thai cultural elements
    bool appropriate_language;     // Uses appropriate formal/informal language
    std::string cultural_category; // Classification of cultural content type
    std::vector<std::string> detected_keywords; // Cultural keywords found
    double cultural_compliance;    // Cultural appropriateness score 0-100
};

// Thai metadata structure with ETSI compliance
struct ThaiMetadata {
    std::string title_thai;        // Thai title in UTF-8
    std::string title_dab;         // DAB profile 0x0E encoded title
    std::string artist_thai;       // Thai artist name
    std::string artist_dab;        // DAB encoded artist
    std::string album_thai;        // Thai album name  
    std::string album_dab;         // DAB encoded album
    std::string genre_thai;        // Thai genre description
    std::string station_name_thai; // Thai station identification
    
    CharacterValidation title_validation;
    CharacterValidation artist_validation;
    CharacterValidation album_validation;
    CharacterValidation genre_validation;
    
    CulturalAnalysis cultural_analysis;
    
    bool has_english_fallback;     // Contains English alternatives
    double overall_compliance;     // Overall Thai compliance score
    std::chrono::system_clock::time_point timestamp;
};

// DLS (Dynamic Label Segment) Thai analysis
struct DLSThaiAnalysis {
    std::string original_text;     // Original DLS text
    std::string thai_portion;      // Extracted Thai text
    std::string english_portion;   // Extracted English text
    bool bilingual;               // Contains both Thai and English
    CharacterValidation validation;
    CulturalAnalysis cultural;
    int segment_length;           // DLS segment length
    bool exceeds_limit;           // Exceeds DAB DLS length limit
    std::vector<std::string> segments; // Split DLS segments if too long
};

// Buddhist Calendar Integration
class BuddhistCalendar {
public:
    BuddhistCalendar();
    
    // Convert Gregorian to Buddhist Era dates
    int getbuddhist_year(int gregorian_year) const;
    std::string format_buddhist_date(const std::chrono::system_clock::time_point& date) const;
    
    // Check for Buddhist holy days and festivals
    bool is_buddhist_holy_day(const std::chrono::system_clock::time_point& date) const;
    bool is_major_festival(const std::chrono::system_clock::time_point& date) const;
    std::string get_festival_name(const std::chrono::system_clock::time_point& date) const;
    
    // Get appropriate content guidelines for dates
    std::vector<std::string> get_content_guidelines(const std::chrono::system_clock::time_point& date) const;
    bool requires_special_handling(const std::chrono::system_clock::time_point& date) const;

private:
    std::set<std::string> holy_days_;
    std::map<std::string, std::string> festivals_;
    std::map<std::string, std::vector<std::string>> content_guidelines_;
    
    void initialize_holy_days();
    void initialize_festivals();
    void initialize_content_guidelines();
};

// Thai Character Set Analyzer
class ThaiCharacterAnalyzer {
public:
    ThaiCharacterAnalyzer();
    
    // Character encoding validation
    CharacterValidation validate_thai_text(const std::string& text) const;
    std::string convert_to_dab_profile(const std::string& utf8_text) const;
    bool is_valid_thai_character(uint32_t codepoint) const;
    bool is_renderable_on_dab(uint32_t codepoint) const;
    
    // Text analysis and classification
    bool detect_thai_script(const std::string& text) const;
    bool detect_mixed_scripts(const std::string& text) const;
    std::pair<std::string, std::string> separate_thai_english(const std::string& text) const;
    
    // ETSI TS 101 756 compliance checking
    bool check_profile_0x0E_compliance(const std::string& text) const;
    std::vector<std::string> get_compliance_issues(const std::string& text) const;
    double calculate_compliance_score(const std::string& text) const;

private:
    std::map<uint32_t, uint8_t> utf8_to_dab_mapping_;
    std::set<uint32_t> valid_thai_codepoints_;
    std::set<uint32_t> renderable_codepoints_;
    
    void initialize_character_mappings();
    void initialize_valid_codepoints();
    uint32_t utf8_to_codepoint(const std::string& text, size_t& pos) const;
};

// Cultural Content Analyzer
class CulturalContentAnalyzer {
public:
    CulturalContentAnalyzer();
    
    // Cultural content analysis
    CulturalAnalysis analyze_cultural_content(const std::string& text) const;
    bool detect_buddhist_content(const std::string& text) const;
    bool detect_royal_content(const std::string& text) const;
    bool detect_traditional_content(const std::string& text) const;
    
    // Language appropriateness checking
    bool check_formal_language_usage(const std::string& text) const;
    bool check_respectful_language(const std::string& text) const;
    std::vector<std::string> detect_inappropriate_content(const std::string& text) const;
    
    // Content classification
    std::string classify_content_type(const std::string& text) const;
    double calculate_cultural_compliance(const std::string& text) const;
    std::vector<std::string> get_content_recommendations(const std::string& text) const;

private:
    std::vector<std::string> buddhist_keywords_;
    std::vector<std::string> royal_keywords_;
    std::vector<std::string> traditional_keywords_;
    std::vector<std::string> inappropriate_terms_;
    std::vector<std::string> formal_indicators_;
    std::map<std::string, std::string> content_classifications_;
    
    void initialize_keyword_databases();
    void initialize_classification_rules();
    bool contains_keywords(const std::string& text, const std::vector<std::string>& keywords) const;
};

// Main Thai Analysis Engine
class ThaiAnalysisEngine {
public:
    ThaiAnalysisEngine();
    ~ThaiAnalysisEngine();
    
    // Primary analysis functions
    ThaiMetadata analyze_fig1_labels(const fig1_label& label) const;
    DLSThaiAnalysis analyze_dls_content(const std::string& dls_text) const;
    
    // FIG-specific Thai analysis
    std::vector<std::string> analyze_fig0_1_thai_labels(const uint8_t* fig_data, int length) const;
    CharacterValidation validate_fig0_2_service_labels(const uint8_t* fig_data, int length) const;
    
    // Comprehensive validation
    ComplianceLevel get_overall_compliance_level(const ThaiMetadata& metadata) const;
    std::string generate_compliance_report(const ThaiMetadata& metadata) const;
    std::string generate_json_report(const ThaiMetadata& metadata) const;
    
    // Real-time monitoring support
    void update_compliance_statistics(const ThaiMetadata& metadata);
    double get_running_compliance_average() const;
    int get_total_analyzed_count() const;
    std::map<std::string, int> get_issue_frequency() const;
    
    // Integration with Buddhist calendar
    bool should_use_special_validation(const std::chrono::system_clock::time_point& date) const;
    std::vector<std::string> get_date_specific_guidelines(const std::chrono::system_clock::time_point& date) const;

private:
    std::unique_ptr<ThaiCharacterAnalyzer> character_analyzer_;
    std::unique_ptr<CulturalContentAnalyzer> cultural_analyzer_;
    std::unique_ptr<BuddhistCalendar> buddhist_calendar_;
    
    // Statistics tracking
    mutable int total_analyzed_;
    mutable double total_compliance_score_;
    mutable std::map<std::string, int> issue_frequency_;
    
    // Helper methods
    bool is_thai_service_label(const fig1_label& label) const;
    std::string extract_thai_text_from_fig(const uint8_t* fig_data, int length) const;
    ComplianceLevel score_to_level(double score) const;
};

// Thai Analysis Integration for etisnoop
class ETISnoopThaiIntegration {
public:
    ETISnoopThaiIntegration();
    
    // Integration with existing etisnoop functions
    void analyze_ensemble_thai_info(WatermarkDecoder& ensemble_db);
    void analyze_service_thai_labels(WatermarkDecoder& ensemble_db);
    void analyze_fig_thai_content(const FIGRuntimeInfo& fig_runtime);
    
    // Compliance reporting
    void generate_thai_compliance_report(const std::string& output_path) const;
    void stream_compliance_to_monitor(const std::string& monitor_url) const;
    
    // Real-time analysis for StreamDAB integration
    void enable_realtime_analysis(const std::string& websocket_url);
    void disable_realtime_analysis();
    bool is_realtime_enabled() const;

private:
    std::unique_ptr<ThaiAnalysisEngine> analysis_engine_;
    std::vector<ThaiMetadata> analyzed_metadata_;
    std::string realtime_websocket_url_;
    bool realtime_enabled_;
    
    void send_realtime_update(const ThaiMetadata& metadata) const;
    void log_thai_analysis_result(const ThaiMetadata& metadata) const;
};

// Utility functions for integration
namespace Utils {
    
// Convert compliance level to string
std::string compliance_level_to_string(ComplianceLevel level);

// Convert compliance level to color code for visualization
std::string compliance_level_to_color(ComplianceLevel level);

// Format Thai text for console output
std::string format_thai_for_console(const std::string& thai_text);

// Generate summary statistics
std::string generate_thai_statistics_summary(const std::vector<ThaiMetadata>& metadata_list);

// YAML output formatting for Thai content
std::string format_thai_for_yaml(const ThaiMetadata& metadata);

// JSON output formatting for StreamDAB integration  
std::string format_thai_for_json(const ThaiMetadata& metadata);

// CSV output formatting for government reporting
std::string format_thai_for_csv(const std::vector<ThaiMetadata>& metadata_list);

} // namespace Utils

} // namespace Thai