CREATE TABLE IF NOT EXISTS Query (
    id SERIAL PRIMARY KEY,
    name VARCHAR(255) NOT NULL,
    prompt TEXT NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

INSERT INTO Query (name, prompt) VALUES
('SummarizeText', 'Summarize the following text: {{content}}'),
('ExplainConcept', 'Explain the following concept in simple terms: {{content}}'),
('TranslateToGerman', 'Translate the following text to German: {{content}}');