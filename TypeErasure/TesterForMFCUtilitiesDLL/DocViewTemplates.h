#pragma once
class DocViewTemplates
{
public:
	DocViewTemplates() = default;
	~DocViewTemplates() = default;

	template<size_t N>
	bool OpenNullFile(const wchar_t (&fileType)[N]);
};

template <size_t N>
bool DocViewTemplates::OpenNullFile(const wchar_t (&fileType)[N])
{
	auto pos = theApp.GetFirstDocTemplatePosition();
	while (pos)
	{
		CMultiDocTemplate* temp = dynamic_cast<CMultiDocTemplate*>(theApp.GetNextDocTemplate(pos));

		CString csFileType{ fileType };

		CString docString;
		temp->GetDocString(docString, CDocTemplate::DocStringIndex::docName);
		if (docString == csFileType)
		{
			temp->OpenDocumentFile(nullptr);
			return true;
		}
	}
	return false;
}
